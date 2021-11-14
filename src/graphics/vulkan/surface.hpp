#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    class Surface final
    {
    public:
        using Format       = VkSurfaceFormatKHR;
        using PresentMode  = VkPresentModeKHR;
        using Capabilities = VkSurfaceCapabilitiesKHR;

        using Formats      = std::vector<Format>;
        using PresentModes = std::vector<PresentMode>;

        Surface() noexcept;
        Surface(const VkInstance instance, const VkSurfaceKHR surface) noexcept;

        Surface(const Surface &surface) = delete;
        Surface(Surface &&surface) noexcept;

        Surface &
        operator=(const Surface &surface) = delete;
        Surface &
        operator=(Surface &&surface) noexcept;

        ~Surface();

        constexpr VkSurfaceKHR
        get() const noexcept;

    private:
        VkInstance   instance_ {VK_NULL_HANDLE};
        VkSurfaceKHR surface_ {VK_NULL_HANDLE};
    };

    constexpr VkSurfaceKHR
    Surface::get() const noexcept
    {
        return surface_;
    }

    Surface::Formats
    getSurfaceFormats(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface) noexcept;

    Surface::PresentModes
    getSurfacePresentModes(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface) noexcept;

    Surface::Capabilities
    getSurfaceCapabilities(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface) noexcept;

    Surface
    getSurface(const VkInstance instance, const VkSurfaceKHR surface) noexcept;
}    // namespace nd::src::graphics::vulkan
