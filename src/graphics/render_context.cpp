#include "render_context.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics
{
    using namespace nd::src::tools;

    using nd::src::graphics::vulkan::getQueue;
    using nd::src::graphics::vulkan::getQueues;
    using nd::src::graphics::vulkan::createFence;
    using nd::src::graphics::vulkan::createFences;
    using nd::src::graphics::vulkan::createSemaphore;
    using nd::src::graphics::vulkan::createSemaphores;
    using nd::src::graphics::vulkan::allocateCommandBuffers;
    using nd::src::graphics::vulkan::allocateDescriptorSets;

    RenderContext
    getRenderContext(vulkan::Objects&       objects,
                     const CommandBufferCfg commandBufferCfg,
                     const u16              threadCount,
                     const u16              frameCount) noexcept(ND_VK_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return RenderContext {
            .semaphore     = {.acquired = createSemaphores(objects, {}, frameCount),
                              .graphics = createSemaphores(objects, {}, frameCount),
                              .transfer = createSemaphores(objects, {}, frameCount),
                              .compute  = createSemaphores(objects, {}, frameCount)},
            .queue         = {.graphics  = getQueues(objects.device.handle,
                                            objects.device.queueFamily.graphics.index,
                                            objects.device.queueFamily.graphics.queueCount),
                              .transfer  = getQueues(objects.device.handle,
                                            objects.device.queueFamily.transfer.index,
                                            objects.device.queueFamily.transfer.queueCount),
                              .compute   = getQueues(objects.device.handle,
                                           objects.device.queueFamily.compute.index,
                                           objects.device.queueFamily.compute.queueCount),
                              .swapchain = getQueues(objects.device.handle, objects.swapchain.queueFamily.index, objects.swapchain.queueFamily.queueCount)},
            .commandBuffer = {.graphics = allocateCommandBuffers({.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .count = commandBufferCfg.graphicsCount},
                                                                 objects.commandPool.graphics,
                                                                 objects.device.handle),
                              .transfer = allocateCommandBuffers({.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .count = commandBufferCfg.transferCount},
                                                                 objects.commandPool.transfer,
                                                                 objects.device.handle),
                              .compute  = allocateCommandBuffers({.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .count = commandBufferCfg.computeCount},
                                                                objects.commandPool.compute,
                                                                objects.device.handle)},
            .descriptorSet = {.mesh = allocateDescriptorSets({.layouts = vec<VkDescriptorSetLayout>(frameCount, objects.descriptorSetLayout.mesh)},
                                                             objects.descriptorPool,
                                                             objects.device.handle)},
            .fence         = {.rendered = createFences(objects, {.flags = VK_FENCE_CREATE_SIGNALED_BIT}, frameCount)}};
    }

    RenderContext::Frame
    getRenderContextFrame(const RenderContext&   renderContext,
                          const CommandBufferCfg commandBufferCfg,
                          const u16              threadCount,
                          const u16              frameCount,
                          const u16              frameIndex) noexcept
    {
        ND_SET_SCOPE();

        const auto graphicsCount  = threadCount * commandBufferCfg.graphicsCount;
        const auto graphicsOffset = graphicsCount * frameIndex;

        const auto transferCount  = threadCount * commandBufferCfg.transferCount;
        const auto transferOffset = transferCount * frameIndex;

        const auto computeCount  = threadCount * commandBufferCfg.computeCount;
        const auto computeOffset = computeCount * frameIndex;

        return RenderContext::Frame {.commandBuffer = {.graphics = span {renderContext.commandBuffer.graphics}.subspan(graphicsOffset, graphicsCount),
                                                       .transfer = span {renderContext.commandBuffer.transfer}.subspan(transferOffset, transferCount),
                                                       .compute  = span {renderContext.commandBuffer.compute}.subspan(computeOffset, computeCount)},
                                     .semaphore     = {.graphics = renderContext.semaphore.graphics[frameIndex],
                                                       .transfer = renderContext.semaphore.transfer[frameIndex],
                                                       .compute  = renderContext.semaphore.compute[frameIndex]},
                                     .fence         = {.rendered = renderContext.fence.rendered[frameIndex]},
                                     .descriptorSet = {.mesh = renderContext.descriptorSet.mesh[frameIndex]}};
    }
} // namespace nd::src::graphics
