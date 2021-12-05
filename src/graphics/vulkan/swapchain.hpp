#pragma once

#include "pch.hpp"
#include "shared.hpp"

#include "queue.hpp"

namespace nd::src::graphics::vulkan
{
    class Swapchain final
    {
    public:
        struct Configuration final
        {
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

        Swapchain() noexcept;
        Swapchain(const VkDevice                         device,
                  const std::vector<const QueueFamily*>& queueFamilies,
                  const VkSwapchainCreateInfoKHR&        createInfo);

        Swapchain(const Swapchain& swapchain) = delete;
        Swapchain(Swapchain&& swapchain) noexcept;

        Swapchain&
        operator=(const Swapchain& swapchain) = delete;
        Swapchain&
        operator=(Swapchain&& swapchain) noexcept;

        ~Swapchain();

        constexpr VkSwapchainKHR
        get() const noexcept;

        constexpr const std::vector<const QueueFamily*>&
        getQueueFamilies() const noexcept;

    private:
        std::vector<const QueueFamily*> queueFamilies_ {};

        VkDevice       device_ {VK_NULL_HANDLE};
        VkSwapchainKHR swapchain_ {VK_NULL_HANDLE};
    };

    constexpr VkSwapchainKHR
    Swapchain::get() const noexcept
    {
        return swapchain_;
    }

    constexpr const std::vector<const QueueFamily*>&
    Swapchain::getQueueFamilies() const noexcept
    {
        return queueFamilies_;
    }

    bool
    isFormatSupported(const Swapchain::Configuration&        configuration,
                      const std::vector<VkSurfaceFormatKHR>& formats) noexcept;

    bool
    isPresentModeSupported(const Swapchain::Configuration&      configuration,
                           const std::vector<VkPresentModeKHR>& presentModes) noexcept;

    bool
    isImageUsageSupported(const Swapchain::Configuration& configuration,
                          const VkSurfaceCapabilitiesKHR& capabilities) noexcept;

    bool
    isTransformSupported(const Swapchain::Configuration& configuration,
                         const VkSurfaceCapabilitiesKHR& capabilities) noexcept;

    bool
    isCompositeAlphaSupported(const Swapchain::Configuration& configuration,
                              const VkSurfaceCapabilitiesKHR& capabilities) noexcept;

    uint32_t
    getMinImagesCount(const Swapchain::Configuration& configuration, const VkSurfaceCapabilitiesKHR& capabilities) noexcept;

    VkExtent2D
    getImageExtent(const Swapchain::Configuration& configuration, const VkSurfaceCapabilitiesKHR& capabilities) noexcept;

    uint32_t
    getImageArrayLayers(const Swapchain::Configuration& configuration,
                        const VkSurfaceCapabilitiesKHR& capabilities) noexcept;

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
                           const VkSwapchainKHR                oldSwapchain) noexcept;

    Swapchain
    getSwapchain(const Swapchain::Configuration& configuration,
                 const std::vector<QueueFamily>& queueFamiliesPool,
                 const VkDevice                  device);

    std::vector<VkImage>
    getSwapchainImages(const VkDevice device, const VkSwapchainKHR swapchain) noexcept;
} // namespace nd::src::graphics::vulkan
