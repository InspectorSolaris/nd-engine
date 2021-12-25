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

    VkQueue
    getQueue(const VkDevice device, const uint32_t queueFamilyIndex, const uint32_t queueIndex) noexcept;

    std::map<uint32_t, std::vector<VkQueue>>
    getQueues(const VkDevice device, const std::vector<QueueFamily>& queueFamilies) noexcept;

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
