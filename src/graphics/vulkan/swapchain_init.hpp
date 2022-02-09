#pragma once

#include "shared_init.hpp"
#include "image_init.hpp"

namespace nd::src::graphics::vulkan
{
    u32
    getNextImageIndex(const VkDevice,
                      const VkSwapchainKHR,
                      const VkSemaphore = VK_NULL_HANDLE,
                      const VkFence     = VK_NULL_HANDLE,
                      const u64         = std::numeric_limits<u64>::max()) noexcept;

    vec<Image>
    getSwapchainImages(const VkDevice, const VkSwapchainKHR) noexcept;

    vec<VkSurfaceFormatKHR>
    getSurfaceFormats(const VkPhysicalDevice, const VkSurfaceKHR) noexcept;

    vec<VkPresentModeKHR>
    getSurfacePresentModes(const VkPhysicalDevice, const VkSurfaceKHR) noexcept;

    VkSurfaceCapabilitiesKHR
    getSurfaceCapabilities(const VkPhysicalDevice, const VkSurfaceKHR) noexcept;

    bool
    isSurfaceQueueFamilySupported(const VkPhysicalDevice, const VkSurfaceKHR, const QueueFamily&) noexcept;

    bool
    isSwapchainImageFormatSupported(opt<const SwapchainCfg>::ref, const vec<VkSurfaceFormatKHR>&) noexcept;

    bool
    isSwapchainImageUsageSupported(opt<const SwapchainCfg>::ref, const VkSurfaceCapabilitiesKHR&) noexcept;

    bool
    isSwapchainTransformSupported(opt<const SwapchainCfg>::ref, const VkSurfaceCapabilitiesKHR&) noexcept;

    bool
    isSwapchainCompositeAlphaSupported(opt<const SwapchainCfg>::ref, const VkSurfaceCapabilitiesKHR&) noexcept;

    bool
    isSwapchainPresentModeSupported(opt<const SwapchainCfg>::ref, const vec<VkPresentModeKHR>&) noexcept;

    u32
    getSwapchainImageCount(opt<const SwapchainCfg>::ref, const VkSurfaceCapabilitiesKHR&) noexcept;

    u32
    getSwapchainImageArrayLayers(opt<const SwapchainCfg>::ref, const VkSurfaceCapabilitiesKHR&) noexcept;

    VkExtent2D
    getSwapchainImageExtent(opt<const SwapchainCfg>::ref, const VkSurfaceCapabilitiesKHR&) noexcept;

    Swapchain
    createSwapchain(opt<const SwapchainCfg>::ref, const VkDevice) noexcept(ND_ASSERT_NOTHROW);

    vec<ImageView>
    createSwapchainImageViews(opt<const ImageViewCfg>::ref, const VkDevice, const vec<VkImage>&) noexcept(ND_ASSERT_NOTHROW);

    vec<Framebuffer>
    createSwapchainFramebuffers(opt<const FramebufferCfg>::ref, const VkDevice, const vec<VkImageView>&) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
