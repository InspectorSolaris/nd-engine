#include "surface.hpp"
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
} // namespace nd::src::graphics::vulkan
