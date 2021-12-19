#pragma once

#include "pch.hpp"
#include "shared.hpp"

#include "queue.hpp"

namespace nd::src::graphics::vulkan
{
    struct SwapchainConfiguration final
    {
        const std::vector<QueueFamily>&        queueFamiliesPool;
        const std::vector<VkSurfaceFormatKHR>& formats;
        const std::vector<VkPresentModeKHR>&   presentModes;
        const VkSurfaceCapabilitiesKHR&        capabilities;

        const VkPhysicalDevice physicalDevice;
        const VkSurfaceKHR     surface;

        const VkExtent2D imageExtent;

        const uint32_t minImagesCount;
        const uint32_t imageArrayLayers;

        const VkBool32          clipped;
        const VkFormat          imageFormat;
        const VkColorSpaceKHR   imageColorSpace;
        const VkImageUsageFlags imageUsage;
        const VkPresentModeKHR  presentMode;

        const VkSurfaceTransformFlagBitsKHR transform;
        const VkCompositeAlphaFlagBitsKHR   compositeAlpha;
    };

    bool
    isFormatSupported(const SwapchainConfiguration& configuration, const std::vector<VkSurfaceFormatKHR>& formats) noexcept;

    bool
    isPresentModeSupported(const SwapchainConfiguration&        configuration,
                           const std::vector<VkPresentModeKHR>& presentModes) noexcept;

    bool
    isImageUsageSupported(const SwapchainConfiguration&   configuration,
                          const VkSurfaceCapabilitiesKHR& capabilities) noexcept;

    bool
    isTransformSupported(const SwapchainConfiguration& configuration, const VkSurfaceCapabilitiesKHR& capabilities) noexcept;

    bool
    isCompositeAlphaSupported(const SwapchainConfiguration&   configuration,
                              const VkSurfaceCapabilitiesKHR& capabilities) noexcept;

    uint32_t
    getMinImagesCount(const SwapchainConfiguration& configuration, const VkSurfaceCapabilitiesKHR& capabilities) noexcept;

    VkExtent2D
    getImageExtent(const SwapchainConfiguration& configuration, const VkSurfaceCapabilitiesKHR& capabilities) noexcept;

    uint32_t
    getImageArrayLayers(const SwapchainConfiguration& configuration, const VkSurfaceCapabilitiesKHR& capabilities) noexcept;

    std::vector<const QueueFamily*>
    getSwapchainQueueFamilies(const std::vector<QueueFamily>& queueFamilies,
                              const VkPhysicalDevice          physicalDevice,
                              const VkSurfaceKHR              surface) noexcept;

    VkSwapchainCreateInfoKHR
    getSwapchainCreateInfo(const VkSurfaceKHR                  surface,
                           const uint32_t                      minImagesCount,
                           const VkFormat                      imageFormat,
                           const VkColorSpaceKHR               imageColorSpace,
                           const VkExtent2D                    imageExtent,
                           const uint32_t                      imageArrayLayers,
                           const VkImageUsageFlags             imageUsage,
                           const VkSharingMode                 imageSharingMode,
                           const uint32_t                      queueFamilyIndicesCount,
                           const uint32_t*                     queueFamilyIndices,
                           const VkSurfaceTransformFlagBitsKHR preTransform,
                           const VkCompositeAlphaFlagBitsKHR   compositeAlpha,
                           const VkPresentModeKHR              presentMode,
                           const VkBool32                      clipped,
                           const VkSwapchainKHR                oldSwapchain,
                           const VkSwapchainCreateFlagsKHR     flags = {},
                           const void*                         next  = {}) noexcept;

    VkSwapchainKHR
    getSwapchain(const VkSwapchainCreateInfoKHR& createInfo, const VkDevice device);

    VkSwapchainKHR
    getSwapchain(const SwapchainConfiguration& configuration, const VkDevice device);

    std::vector<VkImage>
    getSwapchainImages(const VkDevice device, const VkSwapchainKHR swapchain) noexcept;
} // namespace nd::src::graphics::vulkan
