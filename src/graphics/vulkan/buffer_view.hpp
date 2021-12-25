#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    VkBufferViewCreateInfo
    getBufferViewCreateInfo(const VkBuffer                buffer,
                            const VkFormat                format,
                            const VkDeviceSize            offset,
                            const VkDeviceSize            range,
                            const VkBufferViewCreateFlags flags = {},
                            const void*                   next  = {}) noexcept;

    VkBufferView
    getBufferViewHandle(const VkBufferViewCreateInfo& createInfo, const VkDevice device);
} // namespace nd::src::graphics::vulkan
