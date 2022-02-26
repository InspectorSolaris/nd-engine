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

    Scene
    getScene(const Objects& objects, const f64 dt) noexcept
    {
        ND_SET_SCOPE();

        return Scene {
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
    setTransfer(const Objects&              objects,
                const Scene&                scene,
                const RenderContext&        renderContext,
                const RenderContext::Frame& renderContextFrame,
                const f64                   dt) noexcept(ND_VK_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();
    }

    void
    setCompute(const Objects&              objects,
               const Scene&                scene,
               const RenderContext&        renderContext,
               const RenderContext::Frame& renderContextFrame,
               const f64                   dt) noexcept(ND_VK_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();
    }

    void
    setGraphics(const Objects&              objects,
                const Scene&                scene,
                const RenderContext&        renderContext,
                const RenderContext::Frame& renderContextFrame,
                const f64                   dt) noexcept(ND_VK_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();
    }

    void
    draw(Objects& objects, const f64 dt) noexcept(ND_VK_ASSERT_NOTHROW&& ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto threadCount = 1;
        const auto frameCount  = objects.swapchainImages.size();

        const auto memoryLayout = MemoryLayout {.vertex  = {.offset = 0 * 1024, .size = 5 * 1024},
                                                .index   = {.offset = 5 * 1024, .size = 1 * 1024},
                                                .uniform = {.offset = 6 * 1024, .size = 2 * 1024},
                                                .stage   = {.offset = 0 * 1024, .size = 8 * 1024}};

        static auto index  = 0U;
        static auto loaded = false;

        static const auto renderContext = getRenderContext(objects,
                                                           {.graphicsCount = 1, .transferCount = 1, .computeCount = 1},
                                                           threadCount,
                                                           frameCount);

        const auto frameIndex = getNextImageIndex(objects.device.handle, objects.swapchain.handle, renderContext.semaphore.acquired[index]);

        const auto renderContextFrame = getRenderContextFrame(renderContext,
                                                              {.graphicsCount = 1, .transferCount = 1, .computeCount = 1},
                                                              threadCount,
                                                              frameCount,
                                                              frameIndex);

        vkWaitForFences(objects.device.handle, 1, &renderContextFrame.fence.rendered, VK_TRUE, std::numeric_limits<u64>::max());
        vkResetFences(objects.device.handle, 1, &renderContextFrame.fence.rendered);

        resetCommandPools(span {objects.commandPool.graphics}.subspan(frameIndex * threadCount, threadCount), objects.device.handle);
        resetCommandPools(span {objects.commandPool.transfer}.subspan(frameIndex * threadCount, threadCount), objects.device.handle);
        resetCommandPools(span {objects.commandPool.compute}.subspan(frameIndex * threadCount, threadCount), objects.device.handle);

        const auto scene = getScene(objects, dt);

        setTransfer(objects, scene, renderContext, renderContextFrame, dt);
        setCompute(objects, scene, renderContext, renderContextFrame, dt);
        setGraphics(objects, scene, renderContext, renderContextFrame, dt);

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

        static const auto uniforms = vec<Uniform>(frameCount, uniform);

        const auto commandBufferTransferBeginInfo = VkCommandBufferBeginInfo {.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

        ND_VK_ASSERT(vkBeginCommandBuffer(renderContextFrame.commandBuffer.transfer[0], &commandBufferTransferBeginInfo));

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

            vkCmdCopyBuffer(renderContextFrame.commandBuffer.transfer[0],
                            objects.buffer.stage.handle,
                            objects.buffer.mesh.handle,
                            regions.size(),
                            regions.data());

            for(auto index = 0; index < renderContext.descriptorSet.mesh.size(); ++index)
            {
                const auto bufferInfo = VkDescriptorBufferInfo {.buffer = objects.buffer.mesh.handle,
                                                                .offset = sizeof(Uniform) * index + memoryLayout.uniform.offset,
                                                                .range  = sizeof(Uniform)};

                const auto write = VkWriteDescriptorSet {.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                                                         .pNext            = {},
                                                         .dstSet           = renderContext.descriptorSet.mesh[index],
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

        ND_VK_ASSERT(vkEndCommandBuffer(renderContextFrame.commandBuffer.transfer[0]));

        const auto submitInfoTransferCfg = SubmitInfoCfg {.stages           = {},
                                                          .semaphoresWait   = {},
                                                          .semaphoresSignal = array {renderContextFrame.semaphore.transfer},
                                                          .commandBuffers   = array {renderContextFrame.commandBuffer.transfer[0]}};

        const auto submitInfoTransfers = array {getSubmitInfo(submitInfoTransferCfg)};

        vkQueueSubmit(renderContext.queue.transfer[0], submitInfoTransfers.size(), submitInfoTransfers.data(), VK_NULL_HANDLE);

        const auto clearValues = array {VkClearValue {0.0f, 0.0f, 0.0f, 0.0f}};

        const auto commandBufferGraphicsBeginInfo = VkCommandBufferBeginInfo {.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

        const auto renderPassBeginInfo = VkRenderPassBeginInfo {
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass      = objects.renderPass,
            .framebuffer     = objects.swapchainFramebuffers[frameIndex],
            .renderArea      = {.offset = {.x = 0, .y = 0}, .extent = {.width = width, .height = height}},
            .clearValueCount = static_cast<u32>(clearValues.size()),
            .pClearValues    = clearValues.data()};

        const auto vertexBuffers       = array {objects.buffer.mesh.handle};
        const auto vertexBufferOffsets = array {memoryLayout.vertex.offset};
        const auto indexBuffer         = objects.buffer.mesh.handle;
        const auto indexOffset         = memoryLayout.index.offset;

        const auto descriptorSets = array {renderContextFrame.descriptorSet.mesh};

        ND_VK_ASSERT(vkBeginCommandBuffer(renderContextFrame.commandBuffer.graphics[0], &commandBufferGraphicsBeginInfo));

        vkCmdBeginRenderPass(renderContextFrame.commandBuffer.graphics[0], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(renderContextFrame.commandBuffer.graphics[0], VK_PIPELINE_BIND_POINT_GRAPHICS, objects.pipeline.mesh);
        vkCmdBindDescriptorSets(renderContextFrame.commandBuffer.graphics[0],
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                objects.pipelineLayout.mesh,
                                0,
                                descriptorSets.size(),
                                descriptorSets.data(),
                                0,
                                nullptr);

        vkCmdBindVertexBuffers(renderContextFrame.commandBuffer.graphics[0],
                               0,
                               vertexBuffers.size(),
                               vertexBuffers.data(),
                               vertexBufferOffsets.data());
        vkCmdBindIndexBuffer(renderContextFrame.commandBuffer.graphics[0], indexBuffer, indexOffset, VK_INDEX_TYPE_UINT16);

        vkCmdDrawIndexed(renderContextFrame.commandBuffer.graphics[0], indices.size(), 1, 0, 0, 0);

        vkCmdEndRenderPass(renderContextFrame.commandBuffer.graphics[0]);

        ND_VK_ASSERT(vkEndCommandBuffer(renderContextFrame.commandBuffer.graphics[0]));

        const auto submitInfoCfg = SubmitInfoCfg {
            .stages           = array {0U | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0U | VK_PIPELINE_STAGE_VERTEX_INPUT_BIT},
            .semaphoresWait   = array {renderContext.semaphore.acquired[index], renderContextFrame.semaphore.transfer},
            .semaphoresSignal = array {renderContextFrame.semaphore.graphics},
            .commandBuffers   = array {renderContextFrame.commandBuffer.graphics[0]}};

        const auto presentInfoCfg = PresentInfoCfg {.semaphoresWait = array {renderContextFrame.semaphore.graphics},
                                                    .swapchains     = array {objects.swapchain.handle},
                                                    .images         = array {frameIndex}};

        const auto submitInfos = array {getSubmitInfo(submitInfoCfg)};
        const auto presentInfo = getPresentInfo(presentInfoCfg);

        vkQueueSubmit(renderContext.queue.graphics[0], submitInfos.size(), submitInfos.data(), renderContextFrame.fence.rendered);
        vkQueuePresentKHR(renderContext.queue.swapchain[0], &presentInfo);

        index = (index + 1) % frameCount;
    }
} // namespace nd::src::graphics
