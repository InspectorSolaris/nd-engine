#include "surface.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    Surface::Surface() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    Surface::Surface(const VkInstance instance, const VkSurfaceKHR surface) noexcept
        : instance_(instance)
        , surface_(surface)
    {
        ND_SET_SCOPE_LOW();
    }

    Surface::Surface(Surface&& surface) noexcept
        : instance_(std::move(surface.instance_))
        , surface_(std::move(surface.surface_))
    {
        ND_SET_SCOPE_LOW();

        surface.surface_ = VK_NULL_HANDLE;
    }

    Surface&
    Surface::operator=(Surface&& surface) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&surface == this)
        {
            return *this;
        }

        instance_ = std::move(surface.instance_);
        surface_  = std::move(surface.surface_);

        surface.surface_ = VK_NULL_HANDLE;

        return *this;
    }

    Surface::~Surface()
    {
        ND_SET_SCOPE_LOW();

        vkDestroySurfaceKHR(instance_, surface_, nullptr);
    }

    Surface::Formats
    getSurfaceFormats(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface) noexcept
    {
        ND_SET_SCOPE_LOW();

        uint32_t count;

        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, nullptr);

        auto formats = std::vector<VkSurfaceFormatKHR>(count);

        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, formats.data());

        return formats;
    }

    Surface::PresentModes
    getSurfacePresentModes(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface) noexcept
    {
        ND_SET_SCOPE_LOW();

        uint32_t count;

        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, nullptr);

        auto presentModes = std::vector<VkPresentModeKHR>(count);

        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, presentModes.data());

        return presentModes;
    }

    Surface::Capabilities
    getSurfaceCapabilities(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface) noexcept
    {
        ND_SET_SCOPE_LOW();

        VkSurfaceCapabilitiesKHR capabilities;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

        return capabilities;
    }

    Surface
    getSurface(const VkInstance instance, const VkSurfaceKHR surface) noexcept
    {
        ND_SET_SCOPE_LOW();

        return Surface(instance, surface);
    }
} // namespace nd::src::graphics::vulkan
