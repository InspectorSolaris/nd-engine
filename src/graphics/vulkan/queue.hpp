#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct QueueFamily final
    {
        const uint32_t     index;
        const uint32_t     queueCount;
        const VkQueueFlags queueFlags;
    };

    struct SubmitConfiguration final
    {
        const std::vector<VkCommandBuffer>&      commandBuffers;
        const std::vector<VkPipelineStageFlags>& waitDstStageMask;
        const std::vector<VkSemaphore>&          waitSemaphores;
        const std::vector<VkSemaphore>&          signalSemaphores;
    };

    struct PresentConfiguration final
    {
        const std::vector<VkSwapchainKHR>& swapchains;
        const std::vector<VkSemaphore>&    waitSemaphores;
        const std::vector<uint32_t>&       imageIndices;
    };

    std::vector<uint32_t>
    getQueueFamiliesIndices(const std::vector<const QueueFamily*>& queueFamilies) noexcept;

    VkSubmitInfo
    getSubmitInfo(const uint32_t              commandBuffersCount,
                  const uint32_t              waitSemaphoresCount,
                  const uint32_t              signalSemaphoresCount,
                  const VkCommandBuffer*      commandBuffers,
                  const VkPipelineStageFlags* waitDstStageMask,
                  const VkSemaphore*          waitSemaphores,
                  const VkSemaphore*          signalSemaphores,
                  const void*                 next = {}) noexcept;

    VkSubmitInfo
    getSubmitInfo(const SubmitConfiguration& configuration);

    VkPresentInfoKHR
    getPresentInfo(const uint32_t        swapchainsCount,
                   const uint32_t        waitSemaphoresCount,
                   const VkSwapchainKHR* swapchains,
                   const VkSemaphore*    waitSemaphores,
                   const uint32_t*       imageIndices,
                   const void*           next = {}) noexcept;

    VkPresentInfoKHR
    getPresentInfo(const PresentConfiguration& configuration);
} // namespace nd::src::graphics::vulkan
