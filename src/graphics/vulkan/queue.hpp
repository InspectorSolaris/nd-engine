#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    struct QueueFamily final
    {
        const uint32_t     index;
        const VkQueueFlags queueFlags;

        const std::vector<VkQueue> queues;
    };

    struct SubmitConfiguration final
    {
        const gsl::span<const VkCommandBuffer>      commandBuffers;
        const gsl::span<const VkPipelineStageFlags> waitDstStageMask;
        const gsl::span<const VkSemaphore>          waitSemaphores;
        const gsl::span<const VkSemaphore>          signalSemaphores;
    };

    struct PresentConfiguration final
    {
        const gsl::span<const VkSwapchainKHR> swapchains;
        const gsl::span<const VkSemaphore>    waitSemaphores;
        const gsl::span<const uint32_t>       imageIndices;
    };

    std::vector<VkQueueFamilyProperties>
    getQueueFamiliesProperties(const VkPhysicalDevice physicalDevice) noexcept;

    std::vector<QueueFamily>
    getQueueFamilies(const VkPhysicalDevice physicalDevice, const VkDevice device) noexcept;

    std::vector<QueueFamily>
    getQueueFamilies(const VkPhysicalDevice physicalDevice, const VkDevice device, const VkSurfaceKHR surface) noexcept;

    std::vector<QueueFamily>::const_iterator
    getQueueFamily(const std::vector<QueueFamily>& queueFamilies, const VkQueueFlags queueFlags, const VkQueueFlags queueFlagsNot = {}) noexcept;

    VkQueue
    getQueue(const VkDevice device, const uint32_t queueFamilyIndex, const uint32_t queueIndex) noexcept;

    std::vector<VkQueue>
    getQueues(const VkDevice device, const uint32_t queueFamilyIndex, const VkQueueFamilyProperties queueFamilyProperties) noexcept;

    std::vector<uint32_t>
    getQueueFamiliesIndices(const std::vector<QueueFamily>& queueFamilies) noexcept;

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
