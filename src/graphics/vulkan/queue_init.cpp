#include "queue_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VkQueue
    getQueue(const VkDevice device, const u32 queueFamilyIndex, const u32 queueIndex) noexcept
    {
        ND_SET_SCOPE();

        VkQueue queue;

        vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, &queue);

        return queue;
    }

    QueueFamily
    getQueueFamily(opt<const QueueFamilyCfg>::ref cfg, const vec<QueueFamily>& queueFamilies) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto queueFamily = std::find_if(queueFamilies.begin(),
                                              queueFamilies.end(),
                                              [&cfg](const auto queueFamily)
                                              {
                                                  return isContainsAll(queueFamily.queueFlags, cfg.queueFlags) &&
                                                      !isContainsAny(queueFamily.queueFlags, cfg.queueFlagsNot);
                                              });

        ND_ASSERT(queueFamily != queueFamilies.end());

        return *queueFamily;
    }

    SubmitInfo
    getSubmitInfo(opt<const SubmitInfoCfg>::ref cfg) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        ND_ASSERT(cfg.semaphoresWait.size() == cfg.stages.size());

        return {.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .pNext                = cfg.next,
                .waitSemaphoreCount   = static_cast<u32>(cfg.semaphoresWait.size()),
                .pWaitSemaphores      = cfg.semaphoresWait.data(),
                .pWaitDstStageMask    = cfg.stages.data(),
                .commandBufferCount   = static_cast<u32>(cfg.commandBuffers.size()),
                .pCommandBuffers      = cfg.commandBuffers.data(),
                .signalSemaphoreCount = static_cast<u32>(cfg.semaphoresSignal.size()),
                .pSignalSemaphores    = cfg.semaphoresSignal.data()};
    }

    PresentInfo
    getPresentInfo(opt<const PresentInfoCfg>::ref cfg) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        ND_ASSERT(cfg.swapchains.size() == cfg.images.size());

        return VkPresentInfoKHR {.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                                 .pNext              = cfg.next,
                                 .waitSemaphoreCount = static_cast<u32>(cfg.semaphoresWait.size()),
                                 .pWaitSemaphores    = cfg.semaphoresWait.data(),
                                 .swapchainCount     = static_cast<u32>(cfg.swapchains.size()),
                                 .pSwapchains        = cfg.swapchains.data(),
                                 .pImageIndices      = cfg.images.data()};
    }
} // namespace nd::src::graphics::vulkan
