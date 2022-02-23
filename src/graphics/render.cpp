#include "render.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics
{
    using namespace nd::src::tools;

    using nd::src::graphics::vulkan::getQueue;
    using nd::src::graphics::vulkan::getNextImageIndex;
    using nd::src::graphics::vulkan::resetCommandPools;
    using nd::src::graphics::vulkan::allocateDescriptorSets;
    using nd::src::graphics::vulkan::allocateCommandBuffers;

    using nd::src::graphics::vulkan::Objects;
    using nd::src::graphics::vulkan::SubmitInfoCfg;
    using nd::src::graphics::vulkan::PresentInfoCfg;

    struct Memory final
    {
        VkDeviceSize offset;
        VkDeviceSize size;
    };

    struct MemoryLayout final
    {
        Memory vertex;
        Memory index;
        Memory uniform;
        Memory stage;
    };

    struct Uniform final
    {
        glm::mat4 transform;
    };

    void
    setScene(Objects& objects, const f64 dt) noexcept
    {
        ND_SET_SCOPE();

        const auto scene = Scene {
            .camera    = {.location = {2.0f * std::cos(dt / 4), 2.0f * std::sin(dt / 4), 1.0f},
                          .center   = {0.0f, 0.0f, 0.0f},
                          .up       = {0.0f, 0.0f, 1.0f},
                          .fovx     = 90.0f,
                          .ratio    = static_cast<f32>(objects.swapchain.width) / objects.swapchain.height,
                          .near     = 0.1f,
                          .far      = 10.0f},
            .meshes    = {{.indices  = {0, 1, 2, 1, 2, 3, 4, 5, 6, 5, 6, 7, 0, 4, 5, 0, 1, 5, 1, 5, 6, 1, 2, 6, 2, 6, 7, 2, 3, 7, 3, 7, 4, 3, 0, 4},
                           .vertices = {{.position = {+0.5f, +0.5f, +0.5f}, .color = {0.75f, 0.75f, 0.75f}},
                                     {.position = {+0.5f, -0.5f, +0.5f}, .color = {0.75f, 0.75f, 0.75f}},
                                     {.position = {-0.5f, -0.5f, +0.5f}, .color = {0.75f, 0.75f, 0.75f}},
                                     {.position = {-0.5f, +0.5f, +0.5f}, .color = {0.75f, 0.75f, 0.75f}},
                                     {.position = {+0.5f, +0.5f, -0.5f}, .color = {0.75f, 0.75f, 0.75f}},
                                     {.position = {+0.5f, -0.5f, -0.5f}, .color = {0.75f, 0.75f, 0.75f}},
                                     {.position = {-0.5f, -0.5f, -0.5f}, .color = {0.75f, 0.75f, 0.75f}},
                                     {.position = {-0.5f, +0.5f, -0.5f}, .color = {0.75f, 0.75f, 0.75f}}}}},
            .instances = {
                {.transform = {.rotation = {0.0f, 0.0f, 0.0f}, .scalation = {1.0f, 1.0f, 1.0f}, .translation = {0.0f, 0.0f, 0.0f}}, .meshIndex = 0}}};
    }

    void
    draw(Objects& objects, const f64 dt) noexcept(ND_VK_ASSERT_NOTHROW&& ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto threadCount = 1;
        const auto imageCount  = objects.swapchainImages.size();

        const auto memoryLayout = MemoryLayout {.vertex  = {.offset = 0 * 1024, .size = 5 * 1024},
                                                .index   = {.offset = 5 * 1024, .size = 1 * 1024},
                                                .uniform = {.offset = 6 * 1024, .size = 2 * 1024},
                                                .stage   = {.offset = 0 * 1024, .size = 8 * 1024}};

        static auto index  = 0U;
        static auto loaded = false;

        static const auto queueGraphics  = getQueue(objects.device.handle, objects.device.queueFamily.graphics.index, 0);
        static const auto queueTransfer  = getQueue(objects.device.handle, objects.device.queueFamily.transfer.index, 0);
        static const auto queueCompute   = getQueue(objects.device.handle, objects.device.queueFamily.compute.index, 0);
        static const auto queueSwapchain = getQueue(objects.device.handle, objects.swapchain.queueFamily.index, 0);

        static const auto descriptorSetsMesh = allocateDescriptorSets(
            {.layouts = vec<VkDescriptorSetLayout>(imageCount, objects.descriptorSetLayout.mesh)},
            objects.descriptorPool,
            objects.device.handle);

        static const auto commandBuffersGraphics = allocateCommandBuffers({.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .count = 1},
                                                                          objects.commandPool.graphics,
                                                                          objects.device.handle);

        static const auto commandBuffersTransfer = allocateCommandBuffers({.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .count = 1},
                                                                          objects.commandPool.transfer,
                                                                          objects.device.handle);

        static const auto commandBuffersCompute = allocateCommandBuffers({.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .count = 1},
                                                                         objects.commandPool.compute,
                                                                         objects.device.handle);

        static const auto semaphoresAcquired = createSemaphores(objects, {}, imageCount);
        static const auto semaphoresGraphics = createSemaphores(objects, {}, imageCount);
        static const auto semaphoresTransfer = createSemaphores(objects, {}, imageCount);
        static const auto semaphoresCompute  = createSemaphores(objects, {}, imageCount);
        static const auto fencesRendered     = createFences(objects, {.flags = VK_FENCE_CREATE_SIGNALED_BIT}, imageCount);

        const auto imageIndex = getNextImageIndex(objects.device.handle, objects.swapchain.handle, semaphoresAcquired[index]);

        const auto fences = array {fencesRendered[imageIndex]};

        vkWaitForFences(objects.device.handle, fences.size(), fences.data(), VK_TRUE, std::numeric_limits<u64>::max());
        vkResetFences(objects.device.handle, fences.size(), fences.data());

        const auto commandPoolGraphics = span {objects.commandPool.graphics}.subspan(imageIndex * threadCount, threadCount);
        const auto commandPoolTransfer = span {objects.commandPool.transfer}.subspan(imageIndex * threadCount, threadCount);
        const auto commandPoolCompute  = span {objects.commandPool.compute}.subspan(imageIndex * threadCount, threadCount);

        resetCommandPools(commandPoolGraphics, objects.device.handle);
        resetCommandPools(commandPoolTransfer, objects.device.handle);
        resetCommandPools(commandPoolCompute, objects.device.handle);

        const auto commandBufferGraphics = commandBuffersGraphics[imageIndex * threadCount];
        const auto commandBufferTransfer = commandBuffersTransfer[imageIndex * threadCount];
        const auto commandBufferCompute  = commandBuffersCompute[imageIndex * threadCount];

        const auto width  = static_cast<u32>(objects.swapchain.width);
        const auto height = static_cast<u32>(objects.swapchain.height);

        const auto identityMatrix   = glm::mat4(1.0f);
        const auto modelMatrix      = glm::rotate(identityMatrix, static_cast<f32>(dt * glm::radians(90.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
        const auto viewMatrix       = glm::lookAt(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        const auto projectionMatrix = glm::perspective(glm::radians(60.0f), static_cast<f32>(width) / height, 0.1f, 10.0f);
        const auto vulkanMatrix     = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

        const auto uniform = Uniform {.transform = vulkanMatrix * projectionMatrix * viewMatrix * modelMatrix};

        static const auto indices  = vec<Index> {0, 1, 2};
        static const auto vertices = vec<Vertex> {{.position = {+0.0, -0.5, 0.0}, .color = {1.0, 0.0, 0.0}},
                                                  {.position = {+0.5, +0.5, 0.0}, .color = {0.0, 1.0, 0.0}},
                                                  {.position = {-0.5, +0.5, 0.0}, .color = {0.0, 0.0, 1.0}}};

        static const auto uniforms = vec<Uniform>(imageCount, uniform);

        if(!loaded)
        {
            const auto verticesSize = sizeof(Vertex) * vertices.size();
            const auto indicesSize  = sizeof(Index) * indices.size();
            const auto uniformsSize = sizeof(Uniform) * uniforms.size();

            const auto verticesOffset = 0ULL;
            const auto indicesOffset  = verticesOffset + verticesSize;
            const auto uniformsOffset = indicesOffset + indicesSize;

            void* data;

            vkMapMemory(objects.device.handle,
                        objects.device.memory.host.handle,
                        objects.buffer.stage.offset,
                        verticesSize + indicesSize + uniformsSize,
                        {},
                        &data);

            memcpy((i8*)data + verticesOffset, vertices.data(), verticesSize);
            memcpy((i8*)data + indicesOffset, indices.data(), indicesSize);
            memcpy((i8*)data + uniformsOffset, uniforms.data(), uniformsSize);

            vkUnmapMemory(objects.device.handle, objects.device.memory.host.handle);

            const auto regions = array {VkBufferCopy {.srcOffset = verticesOffset, .dstOffset = memoryLayout.vertex.offset, .size = verticesSize},
                                        VkBufferCopy {.srcOffset = indicesOffset, .dstOffset = memoryLayout.index.offset, .size = indicesSize},
                                        VkBufferCopy {.srcOffset = uniformsOffset, .dstOffset = memoryLayout.uniform.offset, .size = uniformsSize}};

            const auto transferCommandBufferBeginInfo = VkCommandBufferBeginInfo {.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

            ND_VK_ASSERT(vkBeginCommandBuffer(commandBufferTransfer, &transferCommandBufferBeginInfo));

            vkCmdCopyBuffer(commandBufferTransfer, objects.buffer.stage.handle, objects.buffer.mesh.handle, regions.size(), regions.data());

            ND_VK_ASSERT(vkEndCommandBuffer(commandBufferTransfer));

            const auto submitInfoCfg = SubmitInfoCfg {.stages           = {},
                                                      .semaphoresWait   = {},
                                                      .semaphoresSignal = {},
                                                      .commandBuffers   = array {commandBufferTransfer}};

            const auto submitInfos = array {getSubmitInfo(submitInfoCfg)};

            vkQueueSubmit(queueTransfer, submitInfos.size(), submitInfos.data(), VK_NULL_HANDLE);

            for(auto index = 0; index < descriptorSetsMesh.size(); ++index)
            {
                const auto bufferInfo = VkDescriptorBufferInfo {.buffer = objects.buffer.mesh.handle,
                                                                .offset = sizeof(Uniform) * index + memoryLayout.uniform.offset,
                                                                .range  = sizeof(Uniform)};

                const auto write = VkWriteDescriptorSet {.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                                                         .pNext            = {},
                                                         .dstSet           = descriptorSetsMesh[index],
                                                         .dstBinding       = 0,
                                                         .dstArrayElement  = 0,
                                                         .descriptorCount  = 1,
                                                         .descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                                         .pImageInfo       = {},
                                                         .pBufferInfo      = &bufferInfo,
                                                         .pTexelBufferView = {}};

                const auto writes = array {write};

                vkUpdateDescriptorSets(objects.device.handle, writes.size(), writes.data(), 0, nullptr);
            }

            loaded = true;
        }

        const auto clearValues = array {VkClearValue {0.0f, 0.0f, 0.0f, 0.0f}};

        const auto graphicsCommandBufferBeginInfo = VkCommandBufferBeginInfo {.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

        const auto renderPassBeginInfo = VkRenderPassBeginInfo {
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass      = objects.renderPass,
            .framebuffer     = objects.swapchainFramebuffers[imageIndex],
            .renderArea      = {.offset = {.x = 0, .y = 0}, .extent = {.width = width, .height = height}},
            .clearValueCount = static_cast<u32>(clearValues.size()),
            .pClearValues    = clearValues.data()};

        const auto vertexBuffers       = array {objects.buffer.mesh.handle};
        const auto vertexBufferOffsets = array {memoryLayout.vertex.offset};
        const auto indexBuffer         = objects.buffer.mesh.handle;
        const auto indexOffset         = memoryLayout.index.offset;

        const auto descriptorSets = array {descriptorSetsMesh[imageIndex]};

        ND_VK_ASSERT(vkBeginCommandBuffer(commandBufferGraphics, &graphicsCommandBufferBeginInfo));

        vkCmdBeginRenderPass(commandBufferGraphics, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBufferGraphics, VK_PIPELINE_BIND_POINT_GRAPHICS, objects.pipeline.mesh);
        vkCmdBindDescriptorSets(commandBufferGraphics,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                objects.pipelineLayout.mesh,
                                0,
                                descriptorSets.size(),
                                descriptorSets.data(),
                                0,
                                nullptr);

        vkCmdBindVertexBuffers(commandBufferGraphics, 0, vertexBuffers.size(), vertexBuffers.data(), vertexBufferOffsets.data());
        vkCmdBindIndexBuffer(commandBufferGraphics, indexBuffer, indexOffset, VK_INDEX_TYPE_UINT16);

        vkCmdDrawIndexed(commandBufferGraphics, indices.size(), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBufferGraphics);

        ND_VK_ASSERT(vkEndCommandBuffer(commandBufferGraphics));

        const auto submitInfoCfg = SubmitInfoCfg {.stages           = array {0U | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
                                                  .semaphoresWait   = array {semaphoresAcquired[index]},
                                                  .semaphoresSignal = array {semaphoresGraphics[imageIndex]},
                                                  .commandBuffers   = array {commandBufferGraphics}};

        const auto presentInfoCfg = PresentInfoCfg {.semaphoresWait = array {semaphoresGraphics[imageIndex]},
                                                    .swapchains     = array {objects.swapchain.handle},
                                                    .images         = array {imageIndex}};

        const auto submitInfos = array {getSubmitInfo(submitInfoCfg)};
        const auto presentInfo = getPresentInfo(presentInfoCfg);

        vkQueueSubmit(queueGraphics, submitInfos.size(), submitInfos.data(), fencesRendered[imageIndex]);
        vkQueuePresentKHR(queueSwapchain, &presentInfo);

        index = (index + 1) % imageCount;
    }
} // namespace nd::src::graphics
