#include "queue.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

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
    getQueueFamilies(const VkPhysicalDevice physicalDevice, const VkDevice device) noexcept
    {
        ND_SET_SCOPE();

        const auto queueFamiliesProperties = getQueueFamiliesProperties(physicalDevice);

        return getMapped<VkQueueFamilyProperties, QueueFamily>(
            queueFamiliesProperties,
            [device](const auto& properties, const auto index)
            {
                return QueueFamily {static_cast<uint32_t>(index), properties.queueFlags, getQueues(device, index, properties)};
            });
    }

    std::vector<QueueFamily>
    getQueueFamilies(const VkPhysicalDevice physicalDevice, const VkDevice device, const VkSurfaceKHR surface) noexcept
    {
        ND_SET_SCOPE();

        const auto queueFamilies = getQueueFamilies(physicalDevice, device);

        return getFiltered<QueueFamily>(queueFamilies,
                                        [physicalDevice, surface](const auto& queueFamily, const auto index)
                                        {
                                            VkBool32 isSupported;

                                            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamily.index, surface, &isSupported);

                                            return isSupported;
                                        });
    }

    std::vector<QueueFamily>::const_iterator
    getQueueFamily(const std::vector<QueueFamily>& queueFamilies, const VkQueueFlags queueFlags, const VkQueueFlags queueFlagsNot) noexcept
    {
        ND_SET_SCOPE();

        return std::find_if(queueFamilies.begin(),
                            queueFamilies.end(),
                            [queueFlags, queueFlagsNot](const auto& queueFamily)
                            {
                                return isSubmask(queueFamily.queueFlags, queueFlags) && isNotSubmask(queueFamily.queueFlags, queueFlagsNot);
                            });
    }

    VkQueue
    getQueue(const VkDevice device, const uint32_t queueFamilyIndex, const uint32_t queueIndex) noexcept
    {
        ND_SET_SCOPE();

        VkQueue queue;

        vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, &queue);

        return queue;
    }

    std::vector<VkQueue>
    getQueues(const VkDevice device, const uint32_t queueFamilyIndex, const VkQueueFamilyProperties queueFamilyProperties) noexcept
    {
        ND_SET_SCOPE();

        return getMapped<VkQueue>(queueFamilyProperties.queueCount,
                                  [device, queueFamilyIndex](const auto queueIndex)
                                  {
                                      return getQueue(device, queueFamilyIndex, queueIndex);
                                  });
    }

    std::vector<uint32_t>
    getQueueFamiliesIndices(const std::vector<QueueFamily>& queueFamilies) noexcept
    {
        ND_SET_SCOPE();

        return getMapped<QueueFamily, uint32_t>(queueFamilies,
                                                [](const auto& queueFamily, const auto index)
                                                {
                                                    return queueFamily.index;
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
