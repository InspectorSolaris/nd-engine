#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    using SubmitInfo  = VkSubmitInfo;
    using PresentInfo = VkPresentInfoKHR;

    struct SubmitInfoCfg final
    {
        gsl::span<const VkPipelineStageFlags> stages;
        gsl::span<const VkSemaphore>          semaphoresWait;
        gsl::span<const VkSemaphore>          semaphoresSignal;
        gsl::span<const VkCommandBuffer>      commandBuffers;

        void* next = {};
    };

    struct PresentInfoCfg final
    {
        gsl::span<const VkSemaphore>    semaphoresWait;
        gsl::span<const VkSwapchainKHR> swapchains;
        gsl::span<const u32>            images;

        void* next = {};
    };

    VkQueue
    getQueue(const VkDevice, const u32, const u32) noexcept;

    QueueFamily
    getQueueFamily(opt<const QueueFamilyCfg>::ref, const vec<QueueFamily>&) noexcept(ND_ASSERT_NOTHROW);

    SubmitInfo getSubmitInfo(opt<const SubmitInfoCfg>::ref) noexcept(ND_ASSERT_NOTHROW);

    PresentInfo getPresentInfo(opt<const PresentInfoCfg>::ref) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
