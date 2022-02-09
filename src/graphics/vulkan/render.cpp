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

    void
    draw(VulkanObjects& objects, const f64 dt) noexcept(ND_ASSERT_NOTHROW)
    {
        using namespace std;

        ND_SET_SCOPE();

        static const auto indices  = vec<Index> {0, 1, 2};
        static const auto vertices = vec<Vertex> {{.position = {-0.5, +0.0, 0.0}, .color = {1.0, 0.0, 0.0}},
                                                  {.position = {+0.5, +0.5, 0.0}, .color = {0.0, 1.0, 0.0}},
                                                  {.position = {-0.5, +0.5, 0.0}, .color = {0.0, 0.0, 1.0}}};

        const auto imageCount = objects.swapchainImages.size();

        const auto device     = objects.device.handle;
        const auto swapchain  = objects.swapchain.handle;
        const auto renderPass = objects.renderPass;

        opt<const QueueFamily>::ref graphicsQueueFamily  = objects.device.queueFamily.graphics;
        opt<const QueueFamily>::ref transferQueueFamily  = objects.device.queueFamily.transfer;
        opt<const QueueFamily>::ref computeQueueFamily   = objects.device.queueFamily.compute;
        opt<const QueueFamily>::ref swapchainQueueFamily = objects.swapchain.queueFamily;

        opt<const DeviceMemory>::ref deviceMemory = objects.device.memory.device;
        opt<const DeviceMemory>::ref hostMemory   = objects.device.memory.host;

        opt<const arr<CommandPool, CommandPoolObjects::graphicsCount>>::ref graphicsCommandPools = objects.commandPool.graphics;
        opt<const arr<CommandPool, CommandPoolObjects::transferCount>>::ref transferCommandPools = objects.commandPool.transfer;
        opt<const arr<CommandPool, CommandPoolObjects::computeCount>>::ref  computeCommandPools  = objects.commandPool.compute;

        const auto commandBufferCfg = CommandBufferCfg {.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .count = 1};

        static const auto graphicsCommandBuffers = allocateCommandBuffers(commandBufferCfg, graphicsCommandPools[0], device);
        static const auto transferCommandBuffers = allocateCommandBuffers(commandBufferCfg, transferCommandPools[0], device);
        static const auto computeCommandBuffers  = allocateCommandBuffers(commandBufferCfg, computeCommandPools[0], device);

        const auto graphicsCommandBuffer = graphicsCommandBuffers[0];
        const auto transferCommandBuffer = transferCommandBuffers[0];
        const auto computeCommandBuffer  = computeCommandBuffers[0];

        static auto       index             = 0ULL;
        static const auto semaphoresAcquire = createSemaphores(objects, {}, imageCount);
        static const auto semaphoresSubmit  = createSemaphores(objects, {}, imageCount);

        const auto imageIndex = getNextImageIndex(device, swapchain, semaphoresAcquire[index]);



        const auto commandBufferBeginInfo = VkCommandBufferBeginInfo {.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

        ND_ASSERT_EXEC(vkBeginCommandBuffer(graphicsCommandBuffer, &commandBufferBeginInfo) == VK_SUCCESS);

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

        ND_ASSERT_EXEC(vkEndCommandBuffer(graphicsCommandBuffer) == VK_SUCCESS);

        const auto submitInfoCfg = SubmitInfoCfg {.stages = std::array {VkPipelineStageFlags {} | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
                                                  .semaphoresWait   = std::array {semaphoresAcquire[index]},
                                                  .semaphoresSignal = std::array {semaphoresSubmit[index]},
                                                  .commandBuffers = std::array {graphicsCommandBuffer, transferCommandBuffer, computeCommandBuffer}};

        const auto submitInfos = std::array {getSubmitInfo(submitInfoCfg)};

        const auto presentInfoCfg = PresentInfoCfg {.semaphoresWait = std::array {semaphoresAcquire[index]},
                                                    .swapchains     = std::array {swapchain},
                                                    .images         = std::array {imageIndex}};

        const auto presentInfo = getPresentInfo(presentInfoCfg);

        const auto graphicsQueue  = getQueue(device, graphicsQueueFamily.index, 0);
        const auto transferQueue  = getQueue(device, transferQueueFamily.index, 0);
        const auto computeQueue   = getQueue(device, computeQueueFamily.index, 0);
        const auto swapchainQueue = getQueue(device, swapchainQueueFamily.index, 0);

        vkQueueSubmit(graphicsQueue, submitInfos.size(), submitInfos.data(), VK_NULL_HANDLE);
        vkQueuePresentKHR(swapchainQueue, &presentInfo);

        resetCommandPools(graphicsCommandPools, device);
        resetCommandPools(transferCommandPools, device);
        resetCommandPools(computeCommandPools, device);

        index = (index + 1) % imageCount;
    }
} // namespace nd::src::graphics::vulkan
