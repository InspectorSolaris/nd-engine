#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    VkBufferCreateInfo
    getBufferCreateInfo(const VkDeviceSize        size,
                        const VkBufferUsageFlags  usage,
                        const VkSharingMode       sharingMode,
                        const uint32_t            queueFamilyIndicesCount,
                        const uint32_t*           queueFamilyIndices,
                        const VkBufferCreateFlags flags = {},
                        const void*               next  = {}) noexcept;

    VkBuffer
    getBuffer(const VkBufferCreateInfo& createInfo, const VkDevice device);
} // namespace nd::src::graphics::vulkan
