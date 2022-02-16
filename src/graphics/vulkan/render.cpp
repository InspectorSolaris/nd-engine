#include "render.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    using Index = u16;

    struct Vertex final
    {
        glm::vec3 position;
        glm::vec3 color;
    };

    struct Uniform final
    {
        glm::mat4 transform;
    };

    void
    draw(VulkanObjects& objects, const f64 dt) noexcept(ND_VK_ASSERT_NOTHROW&& ND_ASSERT_NOTHROW)
    {
        using namespace std;

        ND_SET_SCOPE();

        static auto index  = 0U;
        static auto loaded = false;

        static const auto indices  = vec<Index> {0, 1, 2};
        static const auto vertices = vec<Vertex> {{.position = {+0.0, -0.5, 0.0}, .color = {1.0, 0.0, 0.0}},
                                                  {.position = {+0.5, +0.5, 0.0}, .color = {0.0, 1.0, 0.0}},
                                                  {.position = {-0.5, +0.5, 0.0}, .color = {0.0, 0.0, 1.0}}};

        const auto imageCount = objects.swapchainImages.size();

        const auto device     = objects.device.handle;
        const auto swapchain  = objects.swapchain.handle;
        const auto renderPass = objects.renderPass;

        opt<const DeviceMemory>::ref deviceMemory = objects.device.memory.device;
        opt<const DeviceMemory>::ref hostMemory   = objects.device.memory.host;

        opt<const QueueFamily>::ref graphicsQueueFamily  = objects.device.queueFamily.graphics;
        opt<const QueueFamily>::ref transferQueueFamily  = objects.device.queueFamily.transfer;
        opt<const QueueFamily>::ref computeQueueFamily   = objects.device.queueFamily.compute;
        opt<const QueueFamily>::ref swapchainQueueFamily = objects.swapchain.queueFamily;

        const auto& graphicsCommandPools = objects.commandPool.graphics;
        const auto& transferCommandPools = objects.commandPool.transfer;
        const auto& computeCommandPools  = objects.commandPool.compute;

        static const auto graphicsQueue  = getQueue(device, graphicsQueueFamily.index, 0);
        static const auto transferQueue  = getQueue(device, transferQueueFamily.index, 0);
        static const auto computeQueue   = getQueue(device, computeQueueFamily.index, 0);
        static const auto swapchainQueue = getQueue(device, swapchainQueueFamily.index, 0);

        const auto commandBufferCfg = CommandBufferCfg {.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .count = 1};

        static const auto graphicsCommandBuffers = allocateCommandBuffers(commandBufferCfg, graphicsCommandPools, device);
        static const auto transferCommandBuffers = allocateCommandBuffers(commandBufferCfg, transferCommandPools, device);
        static const auto computeCommandBuffers  = allocateCommandBuffers(commandBufferCfg, computeCommandPools, device);

        static const auto semaphoresAcquire = createSemaphores(objects, {}, imageCount);
        static const auto semaphoresSubmit  = createSemaphores(objects, {}, imageCount);
        static const auto fencesGraphics    = createFences(objects, {}, imageCount);
        static const auto fencesTransfer    = createFences(objects, {}, imageCount);
        static const auto fencesCompute     = createFences(objects, {}, imageCount);

        const auto graphicsCommandBuffer = graphicsCommandBuffers[index];
        const auto transferCommandBuffer = transferCommandBuffers[index];
        const auto computeCommandBuffer  = computeCommandBuffers[index];

        resetCommandPools(graphicsCommandPools, device);
        resetCommandPools(transferCommandPools, device);
        resetCommandPools(computeCommandPools, device);

        const auto imageIndex = getNextImageIndex(device, swapchain, semaphoresAcquire[index]);

        if(!loaded)
        {
            const auto verticesSize = sizeof(Vertex) * vertices.size();
            const auto indicesSize  = sizeof(Index) * indices.size();

            const auto verticesOffset = 0ULL;
            const auto indicesOffset  = verticesOffset + verticesSize;

            const auto offset = VkDeviceSize {objects.buffer.stage.offset};
            const auto size   = VkDeviceSize {indicesSize + verticesSize};

            void* data;

            vkMapMemory(device, hostMemory.handle, offset, size, {}, &data);

            memcpy((i8*)data + verticesOffset, vertices.data(), verticesSize);
            memcpy((i8*)data + indicesOffset, indices.data(), indicesSize);

            vkUnmapMemory(device, hostMemory.handle);

            const auto regions = std::array {
                VkBufferCopy {.srcOffset = verticesOffset, .dstOffset = objects.buffer.mesh.vertex.offset, .size = verticesSize},
                VkBufferCopy {.srcOffset = indicesOffset, .dstOffset = objects.buffer.mesh.index.offset, .size = indicesSize}};

            const auto transferCommandBufferBeginInfo = VkCommandBufferBeginInfo {.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

            vkBeginCommandBuffer(transferCommandBuffer, &transferCommandBufferBeginInfo);

            vkCmdCopyBuffer(transferCommandBuffer, objects.buffer.stage.handle, objects.buffer.mesh.handle, regions.size(), regions.data());

            vkEndCommandBuffer(transferCommandBuffer);

            const auto submitInfoCfg = SubmitInfoCfg {.stages           = {},
                                                      .semaphoresWait   = {},
                                                      .semaphoresSignal = {},
                                                      .commandBuffers   = std::array {transferCommandBuffer}};

            const auto submitInfos = std::array {getSubmitInfo(submitInfoCfg)};

            vkQueueSubmit(transferQueue, submitInfos.size(), submitInfos.data(), fencesTransfer[index]);

            const auto fencesWait = std::array {fencesTransfer[index]};

            vkWaitForFences(device, fencesWait.size(), fencesWait.data(), VK_TRUE, std::numeric_limits<u64>::max());
            vkResetFences(device, fencesWait.size(), fencesWait.data());

            loaded = true;
        }

        const auto graphicsCommandBufferBeginInfo = VkCommandBufferBeginInfo {.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

        ND_VK_ASSERT(vkBeginCommandBuffer(graphicsCommandBuffer, &graphicsCommandBufferBeginInfo));

        const auto width  = static_cast<u32>(objects.swapchain.width);
        const auto height = static_cast<u32>(objects.swapchain.height);

        const auto clearValues = vec<VkClearValue> {{{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f}}};

        const auto renderPassBeginInfo = VkRenderPassBeginInfo {
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass      = renderPass,
            .framebuffer     = objects.swapchainFramebuffers[imageIndex],
            .renderArea      = {.offset = {.x = 0, .y = 0}, .extent = {.width = width, .height = height}},
            .clearValueCount = static_cast<u32>(clearValues.size()),
            .pClearValues    = clearValues.data()};

        vkCmdBeginRenderPass(graphicsCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        const auto vertexBuffers       = std::array {objects.buffer.mesh.handle};
        const auto vertexBufferOffsets = std::array {objects.buffer.mesh.vertex.offset};
        const auto indexBuffer         = objects.buffer.mesh.handle;
        const auto indexOffset         = objects.buffer.mesh.index.offset;

        vkCmdBindPipeline(graphicsCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, objects.pipeline.mesh);
        vkCmdBindVertexBuffers(graphicsCommandBuffer, 0, vertexBuffers.size(), vertexBuffers.data(), vertexBufferOffsets.data());
        vkCmdBindIndexBuffer(graphicsCommandBuffer, indexBuffer, indexOffset, VK_INDEX_TYPE_UINT16);

        vkCmdDrawIndexed(graphicsCommandBuffer, indices.size(), 1, 0, 0, 0);

        vkCmdEndRenderPass(graphicsCommandBuffer);

        ND_VK_ASSERT(vkEndCommandBuffer(graphicsCommandBuffer));

        const auto submitInfoCfg = SubmitInfoCfg {.stages = std::array {VkPipelineStageFlags {} | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
                                                  .semaphoresWait   = std::array {semaphoresAcquire[index]},
                                                  .semaphoresSignal = std::array {semaphoresSubmit[index]},
                                                  .commandBuffers   = std::array {graphicsCommandBuffer}};

        const auto presentInfoCfg = PresentInfoCfg {.semaphoresWait = std::array {semaphoresSubmit[index]},
                                                    .swapchains     = std::array {swapchain},
                                                    .images         = std::array {imageIndex}};

        const auto submitInfos = std::array {getSubmitInfo(submitInfoCfg)};
        const auto presentInfo = getPresentInfo(presentInfoCfg);

        vkQueueSubmit(graphicsQueue, submitInfos.size(), submitInfos.data(), fencesGraphics[index]);
        vkQueuePresentKHR(swapchainQueue, &presentInfo);

        const auto fencesWait = std::array {fencesGraphics[index]};

        vkWaitForFences(device, fencesWait.size(), fencesWait.data(), VK_TRUE, std::numeric_limits<u64>::max());
        vkResetFences(device, fencesWait.size(), fencesWait.data());

        index = (index + 1) % imageCount;
    }
} // namespace nd::src::graphics::vulkan
