#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    using Image = VkImage;

    VkImageCreateInfo
    getImageCreateInfo(const VkImageType           type,
                       const VkFormat              format,
                       const VkExtent3D            extent,
                       const uint32_t              mipLevels,
                       const uint32_t              arrayLevels,
                       const VkSampleCountFlagBits samples,
                       const VkImageTiling         tiling,
                       const VkImageUsageFlags     usage,
                       const VkSharingMode         sharingMode,
                       const uint32_t              queueFamilyIndicesCount,
                       const uint32_t*             queueFamilyIndices,
                       const VkImageLayout         initialLayout,
                       const VkImageCreateFlags    flags = {},
                       const void*                 next  = {}) noexcept;

    VkImage
    getImageHandle(const VkImageCreateInfo& createInfo, const VkDevice device);

    std::vector<Image>
    getSwapchainImages(const VkSwapchainKHR swapchain, const VkDevice device) noexcept;
} // namespace nd::src::graphics::vulkan
