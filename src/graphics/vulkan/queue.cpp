#include "queue.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VkQueue
    getQueue(const VkDevice device, const uint32_t queueFamilyIndex, const uint32_t queueIndex) noexcept
    {
        VkQueue queue;

        vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, &queue);

        return queue;
    }

    std::optional<QueueFamily>
    getQueueFamily(const std::vector<QueueFamily>& queueFamilies, const VkQueueFlags queueFlags) noexcept
    {
        const auto queueFamilyIterator = std::find_if(queueFamilies.begin(),
                                                      queueFamilies.end(),
                                                      [&queueFlags](const auto& queueFamily)
                                                      {
                                                          return isSubmask(queueFamily.queueFlags, queueFlags);
                                                      });

        return queueFamilyIterator != queueFamilies.end() ? std::optional<QueueFamily> {*queueFamilyIterator}
                                                          : std::optional<QueueFamily> {};
    }

    std::optional<QueueFamily>
    getPresentQueueFamily(const std::vector<QueueFamily>& queueFamilies,
                          const VkPhysicalDevice          physicalDevice,
                          const VkSurfaceKHR              surface)
    {
        const auto queueFamilyIterator = std::find_if(
            queueFamilies.begin(),
            queueFamilies.end(),
            [physicalDevice, surface](const auto& queueFamily)
            {
                VkBool32 supported;

                ND_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamily.index, surface, &supported) ==
                          VK_SUCCESS);

                return supported;
            });

        return queueFamilyIterator != queueFamilies.end() ? std::optional<QueueFamily> {*queueFamilyIterator}
                                                          : std::optional<QueueFamily> {};
    }

    std::vector<uint32_t>
    getQueueFamiliesIndices(const std::vector<const QueueFamily*>& queueFamilies) noexcept
    {
        ND_SET_SCOPE();

        return getMapped<const QueueFamily*, uint32_t>(queueFamilies,
                                                       [](const auto& queueFamily, const auto index)
                                                       {
                                                           return queueFamily->index;
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
