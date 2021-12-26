#include "queue.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    std::vector<VkQueueFamilyProperties>
    getQueueFamiliesProperties(const VkPhysicalDevice physicalDevice) noexcept
    {
        ND_SET_SCOPE();

        uint32_t count;

        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);

        auto queueFamilies = std::vector<VkQueueFamilyProperties>(count);

        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, queueFamilies.data());

        return queueFamilies;
    }

    std::vector<QueueFamily>
    getQueueFamilies(const VkPhysicalDevice physicalDevice) noexcept
    {
        ND_SET_SCOPE();

        const auto queueFamiliesProperties = getQueueFamiliesProperties(physicalDevice);

        return getMapped<VkQueueFamilyProperties, QueueFamily>(
            queueFamiliesProperties,
            [](const auto& properties, const auto index)
            {
                return QueueFamily {static_cast<uint32_t>(index), properties.queueCount, properties.queueFlags};
            });
    }

    std::vector<QueueFamily>
    getQueueFamilies(const std::vector<QueueFamily>&                            queueFamilies,
                     const std::function<bool(const QueueFamily, const size_t)> filter) noexcept
    {
        return getFiltered<QueueFamily>(queueFamilies, filter);
    }

    VkQueue
    getQueue(const VkDevice device, const uint32_t queueFamilyIndex, const uint32_t queueIndex) noexcept
    {
        ND_SET_SCOPE();

        VkQueue queue;

        vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, &queue);

        return queue;
    }

    std::map<uint32_t, std::vector<VkQueue>>
    getQueues(const VkDevice device, const std::vector<QueueFamily>& queueFamilies) noexcept
    {
        ND_SET_SCOPE();

        return getMap<QueueFamily, uint32_t, std::vector<VkQueue>>(
            queueFamilies,
            [device](const auto queueFamily, const auto index)
            {
                return queueFamily.index;
            },
            [device](const auto queueFamily, const auto index)
            {
                return getMapped<VkQueue>(queueFamily.queueCount,
                                          std::bind(getQueue, device, queueFamily.index, std::placeholders::_1));
            });
    }

    VkSubmitInfo
    getSubmitInfo(const uint32_t              commandBuffersCount,
                  const uint32_t              waitSemaphoresCount,
                  const uint32_t              signalSemaphoresCount,
                  const VkCommandBuffer*      commandBuffers,
                  const VkPipelineStageFlags* waitDstStageMask,
                  const VkSemaphore*          waitSemaphores,
                  const VkSemaphore*          signalSemaphores,
                  const void*                 next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_SUBMIT_INFO, // sType;
            next,                          // pNext;
            waitSemaphoresCount,           // waitSemaphoreCount;
            waitSemaphores,                // pWaitSemaphores;
            waitDstStageMask,              // pWaitDstStageMask;
            commandBuffersCount,           // commandBufferCount;
            commandBuffers,                // pCommandBuffers;
            signalSemaphoresCount,         // signalSemaphoreCount;
            signalSemaphores               // pSignalSemaphores;
        };
    }

    VkSubmitInfo
    getSubmitInfo(const SubmitConfiguration& configuration)
    {
        ND_SET_SCOPE();

        ND_ASSERT(configuration.waitSemaphores.size() == configuration.waitDstStageMask.size());

        return getSubmitInfo(configuration.commandBuffers.size(),
                             configuration.waitSemaphores.size(),
                             configuration.signalSemaphores.size(),
                             configuration.commandBuffers.data(),
                             configuration.waitDstStageMask.data(),
                             configuration.waitSemaphores.data(),
                             configuration.signalSemaphores.data());
    }

    VkPresentInfoKHR
    getPresentInfo(const uint32_t        swapchainsCount,
                   const uint32_t        waitSemaphoresCount,
                   const VkSwapchainKHR* swapchains,
                   const VkSemaphore*    waitSemaphores,
                   const uint32_t*       imageIndices,
                   const void*           next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_PRESENT_INFO_KHR, // sType;
            next,                               // pNext;
            waitSemaphoresCount,                // waitSemaphoreCount;
            waitSemaphores,                     // pWaitSemaphores;
            swapchainsCount,                    // swapchainCount;
            swapchains,                         // pSwapchains;
            imageIndices,                       // pImageIndices;
            nullptr                             // pResults;
        };
    }

    VkPresentInfoKHR
    getPresentInfo(const PresentConfiguration& configuration)
    {
        ND_SET_SCOPE();

        ND_ASSERT(configuration.swapchains.size() == configuration.imageIndices.size());

        return getPresentInfo(configuration.swapchains.size(),
                              configuration.waitSemaphores.size(),
                              configuration.swapchains.data(),
                              configuration.waitSemaphores.data(),
                              configuration.imageIndices.data());
    }
} // namespace nd::src::graphics::vulkan
