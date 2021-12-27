#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct ImageViewConfiguration final
    {
        const VkImageSubresourceRange subresourceRange;
        const VkComponentMapping      components;
        const VkImageViewType         viewType;
        const VkFormat                format;
        const VkImage                 image;

        const VkImageViewCreateFlags flags = {};
        const void*                  next  = {};
    };

    struct ImageView final
    {
        const VkImageView handle;
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
    getImageViewHandle(const VkImageViewCreateInfo& createInfo, const VkDevice device);

    ImageView
    getImageView(const ImageViewConfiguration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
