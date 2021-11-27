#pragma once

#include "pch.hpp"
#include "shared.hpp"

#include "device.hpp"
#include "surface.hpp"
#include "image_view.hpp"
#include "framebuffer.hpp"

namespace nd::src::graphics::vulkan
{
    class Swapchain final
    {
    public:
        using CreateInfo = VkSwapchainCreateInfoKHR;

        using QueueFamily           = const Device::QueueFamily*;
        using QueueFamilies         = std::vector<QueueFamily>;
        using QueueFamilyIndex      = uint32_t;
        using QueueFamiliesIndicies = std::vector<QueueFamilyIndex>;
        using Images                = std::vector<VkImage>;
        using ImageViews            = std::vector<ImageView>;
        using Framebuffers          = std::vector<Framebuffer>;

        struct Configuration final
        {
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

        Swapchain() noexcept;
        Swapchain(const VkDevice device, const QueueFamilies& queueFamilies, const CreateInfo& createInfo);

        Swapchain(const Swapchain& swapchain) = delete;
        Swapchain(Swapchain&& swapchain) noexcept;

        Swapchain&
        operator=(const Swapchain& swapchain) = delete;
        Swapchain&
        operator=(Swapchain&& swapchain) noexcept;

        ~Swapchain();

        constexpr VkSwapchainKHR
        get() const noexcept;

        constexpr const QueueFamilies&
        getQueueFamilies() const noexcept;

    private:
        QueueFamilies queueFamilies_ {};

        VkDevice       device_ {VK_NULL_HANDLE};
        VkSwapchainKHR swapchain_ {VK_NULL_HANDLE};
    };

    constexpr VkSwapchainKHR
    Swapchain::get() const noexcept
    {
        return swapchain_;
    }

    constexpr const Swapchain::QueueFamilies&
    Swapchain::getQueueFamilies() const noexcept
    {
        return queueFamilies_;
    }

    bool
    isFormatSupported(const Swapchain::Configuration& configuration, const Surface::Formats& formats) noexcept;

    bool
    isPresentModeSupported(const Swapchain::Configuration& configuration,
                           const Surface::PresentModes&    presentModes) noexcept;

    bool
    isImageUsageSupported(const Swapchain::Configuration& configuration, const Surface::Capabilities& capabilities) noexcept;

    bool
    isTransformSupported(const Swapchain::Configuration& configuration, const Surface::Capabilities& capabilities) noexcept;

    bool
    isCompositeAlphaSupported(const Swapchain::Configuration& configuration,
                              const Surface::Capabilities&    capabilities) noexcept;

    uint32_t
    getMinImagesCount(const Swapchain::Configuration& configuration, const Surface::Capabilities& capabilities) noexcept;

    VkExtent2D
    getImageExtent(const Swapchain::Configuration& configuration, const Surface::Capabilities& capabilities) noexcept;

    uint32_t
    getImageArrayLayers(const Swapchain::Configuration& configuration, const Surface::Capabilities& capabilities) noexcept;

    Swapchain::QueueFamilies
    getSwapchainQueueFamilies(const Device::QueueFamilies& deviceQueueFamilies,
                              const VkPhysicalDevice       physicalDevice,
                              const VkSurfaceKHR           surface) noexcept;

    Swapchain::QueueFamiliesIndicies
    getSwapchainQueueFamiliesIndices(const Swapchain::QueueFamilies& swapchainQueueFamilies) noexcept;

    Swapchain::CreateInfo
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
                           const VkSwapchainKHR                oldSwapchain) noexcept;

    Swapchain
    getSwapchain(const Swapchain::Configuration& configuration,
                 const Device::QueueFamilies&    deviceQueueFamilies,
                 const VkPhysicalDevice          physicalDevice,
                 const VkDevice                  device,
                 const VkSurfaceKHR              surface);

    Swapchain::Images
    getSwapchainImages(const VkDevice device, const VkSwapchainKHR swapchain) noexcept;

    Swapchain::ImageViews
    getSwapchainImageViews(const VkDevice                  device,
                           const Swapchain::Images&        images,
                           const Swapchain::Configuration& configuration) noexcept;

    Swapchain::Framebuffers
    getSwapchainFramebuffers(const VkDevice                  device,
                             const VkRenderPass              renderPass,
                             const Swapchain::ImageViews&    imageViews,
                             const Swapchain::Configuration& configuration) noexcept;
} // namespace nd::src::graphics::vulkan
