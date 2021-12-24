#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct ImageViewConfiguration final
    {
        const VkImageSubresourceRange& subresourceRange;
        const VkComponentMapping&      components;
        const VkImageViewType          viewType;
        const VkFormat                 format;
        const VkImage                  image;
    };

    VkImageViewCreateInfo
    getImageViewCreateInfo(const VkImage                  image,
                           const VkImageViewType          viewType,
                           const VkFormat                 format,
                           const VkComponentMapping&      components,
                           const VkImageSubresourceRange& subresourceRange,
                           const VkImageViewCreateFlags   flags = {},
                           const void*                    next  = {}) noexcept;

    VkImageView
    getImageView(const VkImageViewCreateInfo& createInfo, const VkDevice device);

    VkImageView
    getImageView(const ImageViewConfiguration& configuration,
                 const VkDevice                device,
                 const VkImageViewCreateFlags  flags = {},
                 const void*                   next  = {});
} // namespace nd::src::graphics::vulkan
