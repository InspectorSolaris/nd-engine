#include "swapchain.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    std::vector<VkSurfaceFormatKHR>
    getSurfaceFormats(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface) noexcept
    {
        ND_SET_SCOPE();

        uint32_t count;

        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, nullptr);

        auto formats = std::vector<VkSurfaceFormatKHR>(count);

        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, formats.data());

        return formats;
    }

    std::vector<VkPresentModeKHR>
    getSurfacePresentModes(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface) noexcept
    {
        ND_SET_SCOPE();

        uint32_t count;

        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, nullptr);

        auto presentModes = std::vector<VkPresentModeKHR>(count);

        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, presentModes.data());

        return presentModes;
    }

    VkSurfaceCapabilitiesKHR
    getSurfaceCapabilities(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface) noexcept
    {
        ND_SET_SCOPE();

        VkSurfaceCapabilitiesKHR capabilities;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

        return capabilities;
    }

    bool
    isFormatSupported(const SwapchainConfiguration& configuration, const std::vector<VkSurfaceFormatKHR>& formats) noexcept
    {
        ND_SET_SCOPE();

        return std::any_of(formats.begin(),
                           formats.end(),
                           [&configuration](const auto& format)
                           {
                               return configuration.imageFormat == format.format &&
                                   configuration.imageColorSpace == format.colorSpace;
                           });
    }

    bool
    isPresentModeSupported(const SwapchainConfiguration&        configuration,
                           const std::vector<VkPresentModeKHR>& presentModes) noexcept
    {
        ND_SET_SCOPE();

        return std::any_of(presentModes.begin(),
                           presentModes.end(),
                           [&configuration](const auto& presentMode)
                           {
                               return configuration.presentMode == presentMode;
                           });
    }

    bool
    isImageUsageSupported(const SwapchainConfiguration& configuration, const VkSurfaceCapabilitiesKHR& capabilities) noexcept
    {
        ND_SET_SCOPE();

        return isSubmask(capabilities.supportedUsageFlags, configuration.imageUsage);
    }

    bool
    isTransformSupported(const SwapchainConfiguration& configuration, const VkSurfaceCapabilitiesKHR& capabilities) noexcept
    {
        ND_SET_SCOPE();

        return isSubmask(capabilities.supportedTransforms, configuration.transform);
    }

    bool
    isCompositeAlphaSupported(const SwapchainConfiguration&   configuration,
                              const VkSurfaceCapabilitiesKHR& capabilities) noexcept
    {
        ND_SET_SCOPE();

        return isSubmask(capabilities.supportedCompositeAlpha, configuration.compositeAlpha);
    }

    uint32_t
    getMinImagesCount(const SwapchainConfiguration& configuration, const VkSurfaceCapabilitiesKHR& capabilities) noexcept
    {
        ND_SET_SCOPE();

        return capabilities.maxImageCount
            ? std::clamp(configuration.minImagesCount, capabilities.minImageCount, capabilities.maxImageCount)
            : std::max(capabilities.minImageCount, configuration.minImagesCount);
    }

    VkExtent2D
    getImageExtent(const SwapchainConfiguration& configuration, const VkSurfaceCapabilitiesKHR& capabilities) noexcept
    {
        ND_SET_SCOPE();

        return capabilities.currentExtent.width != 0xFFFFFFFF || capabilities.currentExtent.height != 0xFFFFFFFF
            ? capabilities.currentExtent
            : VkExtent2D {std::clamp(configuration.imageExtent.width,
                                     capabilities.minImageExtent.width,
                                     capabilities.maxImageExtent.width),
                          std::clamp(configuration.imageExtent.height,
                                     capabilities.minImageExtent.height,
                                     capabilities.maxImageExtent.height)};
    }

    uint32_t
    getImageArrayLayers(const SwapchainConfiguration& configuration, const VkSurfaceCapabilitiesKHR& capabilities) noexcept
    {
        ND_SET_SCOPE();

        return std::clamp(configuration.imageArrayLayers, 1U, capabilities.maxImageArrayLayers);
    }

    std::vector<QueueFamily>
    getSwapchainQueueFamilies(const std::vector<QueueFamily>& queueFamiliesPool,
                              const VkPhysicalDevice          physicalDevice,
                              const VkSurfaceKHR              surface) noexcept
    {
        ND_SET_SCOPE();

        auto swapchainQueueFamilies = std::vector<QueueFamily> {};

        for(const auto queueFamily: queueFamiliesPool)
        {
            VkBool32 isSupported;

            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamily.index, surface, &isSupported);

            if(isSupported)
            {
                swapchainQueueFamilies.push_back(queueFamily);
            }
        }

        return swapchainQueueFamilies;
    }

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
                           const VkSwapchainCreateFlagsKHR     flags,
                           const void*                         next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR, // sType;
            next,                                        // pNext;
            flags,                                       // flags;
            surface,                                     // surface;
            minImagesCount,                              // minImageCount;
            imageFormat,                                 // imageFormat;
            imageColorSpace,                             // imageColorSpace;
            imageExtent,                                 // imageExtent;
            imageArrayLayers,                            // imageArrayLayers;
            imageUsage,                                  // imageUsage;
            imageSharingMode,                            // imageSharingMode;
            queueFamilyIndicesCount,                     // queueFamilyIndexCount;
            queueFamilyIndices,                          // pQueueFamilyIndices;
            preTransform,                                // preTransform;
            compositeAlpha,                              // compositeAlpha;
            presentMode,                                 // presentMode;
            clipped,                                     // clipped;
            oldSwapchain,                                // oldSwapchain;
        };
    }

    VkSwapchainKHR
    getSwapchainHandle(const VkSwapchainCreateInfoKHR& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkSwapchainKHR swapchain;

        ND_ASSERT(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain) == VK_SUCCESS);

        return swapchain;
    }

    VkSwapchainKHR
    getSwapchain(const SwapchainConfiguration&   configuration,
                 const VkDevice                  device,
                 const VkSwapchainCreateFlagsKHR flags,
                 const void*                     next)
    {
        ND_SET_SCOPE();

        const auto formats      = getSurfaceFormats(configuration.physicalDevice, configuration.surface);
        const auto presentModes = getSurfacePresentModes(configuration.physicalDevice, configuration.surface);
        const auto capabilities = getSurfaceCapabilities(configuration.physicalDevice, configuration.surface);

        const auto queueFamiliesIndices = getMapped<QueueFamily, uint32_t>(configuration.queueFamiliesPool,
                                                                           [](const auto& queueFamily, const auto index)
                                                                           {
                                                                               return queueFamily.index;
                                                                           });

        ND_ASSERT(queueFamiliesIndices.size());
        ND_ASSERT(isFormatSupported(configuration, formats));
        ND_ASSERT(isPresentModeSupported(configuration, presentModes));
        ND_ASSERT(isImageUsageSupported(configuration, capabilities));
        ND_ASSERT(isTransformSupported(configuration, capabilities));
        ND_ASSERT(isCompositeAlphaSupported(configuration, capabilities));

        const auto minImagesCount   = getMinImagesCount(configuration, capabilities);
        const auto imageExtent      = getImageExtent(configuration, capabilities);
        const auto imageArrayLayers = getImageArrayLayers(configuration, capabilities);

        const auto createInfo =
            getSwapchainCreateInfo(configuration.surface,
                                   minImagesCount,
                                   configuration.imageFormat,
                                   configuration.imageColorSpace,
                                   imageExtent,
                                   imageArrayLayers,
                                   configuration.imageUsage,
                                   queueFamiliesIndices.size() > 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
                                   queueFamiliesIndices.size(),
                                   queueFamiliesIndices.data(),
                                   configuration.transform,
                                   configuration.compositeAlpha,
                                   configuration.presentMode,
                                   configuration.clipped,
                                   VK_NULL_HANDLE,
                                   flags,
                                   next);

        return getSwapchainHandle(createInfo, device);
    }

    std::vector<VkImage>
    getSwapchainImages(const VkDevice device, const VkSwapchainKHR swapchain) noexcept
    {
        ND_SET_SCOPE();

        uint32_t count;

        vkGetSwapchainImagesKHR(device, swapchain, &count, nullptr);

        auto images = std::vector<VkImage>(count);

        vkGetSwapchainImagesKHR(device, swapchain, &count, images.data());

        return images;
    }

    uint32_t
    getNextSwapchainImage(const VkDevice       device,
                          const VkSwapchainKHR swapchain,
                          const VkSemaphore    semaphore,
                          const VkFence        fence,
                          const uint64_t       timeout)
    {
        ND_SET_SCOPE();

        uint32_t imageIndex;

        ND_ASSERT(vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, &imageIndex) == VK_SUCCESS);

        return imageIndex;
    }
} // namespace nd::src::graphics::vulkan
