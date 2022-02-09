#include "swapchain_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    u32
    getNextImageIndex(const VkDevice       device,
                      const VkSwapchainKHR swapchain,
                      const VkSemaphore    semaphore,
                      const VkFence        fence,
                      const u64            timeout) noexcept
    {
        ND_SET_SCOPE();

        u32 index;

        vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, &index);

        return index;
    }

    vec<Image>
    getSwapchainImages(const VkDevice device, const VkSwapchainKHR swapchain) noexcept
    {
        ND_SET_SCOPE();

        u32 count;

        vkGetSwapchainImagesKHR(device, swapchain, &count, nullptr);

        auto images = vec<Image>(count);

        vkGetSwapchainImagesKHR(device, swapchain, &count, images.data());

        return images;
    }

    vec<VkSurfaceFormatKHR>
    getSurfaceFormats(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface) noexcept
    {
        ND_SET_SCOPE();

        u32 count;

        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, nullptr);

        auto formats = vec<VkSurfaceFormatKHR>(count);

        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, formats.data());

        return formats;
    }

    vec<VkPresentModeKHR>
    getSurfacePresentModes(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface) noexcept
    {
        ND_SET_SCOPE();

        u32 count;

        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, nullptr);

        auto presentModes = vec<VkPresentModeKHR>(count);

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
    isSurfaceQueueFamilySupported(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface, const QueueFamily& queueFamily) noexcept
    {
        ND_SET_SCOPE();

        VkBool32 supported;

        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamily.index, surface, &supported);

        return supported;
    }

    bool
    isSwapchainImageFormatSupported(opt<const SwapchainCfg>::ref cfg, const vec<VkSurfaceFormatKHR>& formats) noexcept
    {
        ND_SET_SCOPE();

        return std::any_of(formats.begin(),
                           formats.end(),
                           [&cfg](const auto format)
                           {
                               return format.format == cfg.imageFormat && format.colorSpace == cfg.imageColorSpace;
                           });
    }

    bool
    isSwapchainImageUsageSupported(opt<const SwapchainCfg>::ref cfg, const VkSurfaceCapabilitiesKHR& capabilities) noexcept
    {
        ND_SET_SCOPE();

        return isContainsAll(capabilities.supportedUsageFlags, cfg.imageUsage);
    }

    bool
    isSwapchainTransformSupported(opt<const SwapchainCfg>::ref cfg, const VkSurfaceCapabilitiesKHR& capabilities) noexcept
    {
        ND_SET_SCOPE();

        return isContainsAny(capabilities.supportedTransforms, cfg.transform);
    }

    bool
    isSwapchainCompositeAlphaSupported(opt<const SwapchainCfg>::ref cfg, const VkSurfaceCapabilitiesKHR& capabilities) noexcept
    {
        ND_SET_SCOPE();

        return isContainsAny(capabilities.supportedCompositeAlpha, cfg.compositeAlpha);
    }

    bool
    isSwapchainPresentModeSupported(opt<const SwapchainCfg>::ref cfg, const vec<VkPresentModeKHR>& presentModes) noexcept
    {
        ND_SET_SCOPE();

        return std::any_of(presentModes.begin(),
                           presentModes.end(),
                           [&cfg](const auto presentMode)
                           {
                               return presentMode == cfg.presentMode;
                           });
    }

    u32
    getSwapchainImageCount(opt<const SwapchainCfg>::ref cfg, const VkSurfaceCapabilitiesKHR& capabilities) noexcept
    {
        ND_SET_SCOPE();

        return capabilities.maxImageCount ? std::clamp(cfg.imageCount + 1, capabilities.minImageCount, capabilities.maxImageCount)
                                          : std::min(capabilities.minImageCount, cfg.imageCount + 1);
    }

    u32
    getSwapchainImageArrayLayers(opt<const SwapchainCfg>::ref cfg, const VkSurfaceCapabilitiesKHR& capabilities) noexcept
    {
        ND_SET_SCOPE();

        return std::min(cfg.imageArrayLayers, capabilities.maxImageArrayLayers);
    }

    VkExtent2D
    getSwapchainImageExtent(opt<const SwapchainCfg>::ref cfg, const VkSurfaceCapabilitiesKHR& capabilities) noexcept
    {
        ND_SET_SCOPE();

        const auto anyExtent = capabilities.currentExtent.width == 0xFFFFFFFF && capabilities.currentExtent.height == 0xFFFFFFFF;

        return anyExtent ? VkExtent2D {std::clamp(cfg.imageExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
                                       std::clamp(cfg.imageExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)}
                         : capabilities.currentExtent;
    }

    Swapchain
    createSwapchain(opt<const SwapchainCfg>::ref cfg, const VkDevice device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto formats      = getSurfaceFormats(cfg.physicalDevice, cfg.surface);
        const auto presentModes = getSurfacePresentModes(cfg.physicalDevice, cfg.surface);
        const auto capabilities = getSurfaceCapabilities(cfg.physicalDevice, cfg.surface);

        ND_ASSERT(isSurfaceQueueFamilySupported(cfg.physicalDevice, cfg.surface, cfg.queueFamily.graphics));
        ND_ASSERT(isSwapchainImageFormatSupported(cfg, formats));
        ND_ASSERT(isSwapchainImageUsageSupported(cfg, capabilities));
        ND_ASSERT(isSwapchainTransformSupported(cfg, capabilities));
        ND_ASSERT(isSwapchainCompositeAlphaSupported(cfg, capabilities));
        ND_ASSERT(isSwapchainPresentModeSupported(cfg, presentModes));

        const auto imageCount       = getSwapchainImageCount(cfg, capabilities);
        const auto imageArrayLayers = getSwapchainImageArrayLayers(cfg, capabilities);
        const auto imageExtent      = getSwapchainImageExtent(cfg, capabilities);

        const auto queueFamilyIndices = std::array {static_cast<u32>(cfg.queueFamily.graphics.index)};

        const auto createInfo = VkSwapchainCreateInfoKHR {
            .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext                 = cfg.next,
            .flags                 = cfg.flags,
            .surface               = cfg.surface,
            .minImageCount         = imageCount,
            .imageFormat           = cfg.imageFormat,
            .imageColorSpace       = cfg.imageColorSpace,
            .imageExtent           = imageExtent,
            .imageArrayLayers      = imageArrayLayers,
            .imageUsage            = cfg.imageUsage,
            .imageSharingMode      = queueFamilyIndices.size() <= 1 ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
            .queueFamilyIndexCount = queueFamilyIndices.size(),
            .pQueueFamilyIndices   = queueFamilyIndices.data(),
            .preTransform          = cfg.transform,
            .compositeAlpha        = cfg.compositeAlpha,
            .presentMode           = cfg.presentMode,
            .clipped               = cfg.clipped,
            .oldSwapchain          = VK_NULL_HANDLE};

        VkSwapchainKHR swapchain;

        ND_ASSERT_EXEC(vkCreateSwapchainKHR(device, &createInfo, ND_VULKAN_ALLOCATION_CALLBACKS, &swapchain) == VK_SUCCESS);

        return {.queueFamily = cfg.queueFamily.graphics,
                .handle      = swapchain,
                .width       = static_cast<u16>(cfg.imageExtent.width),
                .height      = static_cast<u16>(cfg.imageExtent.height)};
    }

    vec<ImageView>
    createSwapchainImageViews(opt<const ImageViewCfg>::ref cfg,
                              const VkDevice               device,
                              const vec<VkImage>&          swapchainImages) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return getMapped<Image, ImageView>(swapchainImages,
                                           [&cfg, device](const auto swapchainImage, const auto index)
                                           {
                                               return createImageView(cfg, device, swapchainImage);
                                           });
    }

    vec<Framebuffer>
    createSwapchainFramebuffers(opt<const FramebufferCfg>::ref cfg,
                                const VkDevice                 device,
                                const vec<VkImageView>&        swapchainImageViews) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return getMapped<ImageView, Framebuffer>(swapchainImageViews,
                                                 [&cfg, device](const auto swapchainImageView, const auto index)
                                                 {
                                                     return createFramebuffer(cfg, device, {swapchainImageView});
                                                 });
    }
} // namespace nd::src::graphics::vulkan
