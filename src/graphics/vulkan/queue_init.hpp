#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    using SubmitInfo  = VkSubmitInfo;
    using PresentInfo = VkPresentInfoKHR;

    struct SubmitInfoCfg final
    {
        span<const VkPipelineStageFlags> stages;
        span<const VkSemaphore>          semaphoresWait;
        span<const VkSemaphore>          semaphoresSignal;
        span<const VkCommandBuffer>      commandBuffers;

        void* next = {};
    };

    struct PresentInfoCfg final
    {
        span<const VkSemaphore>    semaphoresWait;
        span<const VkSwapchainKHR> swapchains;
        span<const u32>            images;

        void* next = {};
    };

    VkQueue
    getQueue(const VkDevice, const u32, const u32) noexcept;

    QueueFamily
    getQueueFamily(opt<const QueueFamilyCfg>::ref, const vec<QueueFamily>&) noexcept(ND_ASSERT_NOTHROW);

    SubmitInfo getSubmitInfo(opt<const SubmitInfoCfg>::ref) noexcept(ND_ASSERT_NOTHROW);

    PresentInfo getPresentInfo(opt<const PresentInfoCfg>::ref) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
