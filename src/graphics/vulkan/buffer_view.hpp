#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    struct BufferViewConfiguration final
    {
        const VkBuffer     buffer;
        const VkFormat     format;
        const VkDeviceSize offset;
        const VkDeviceSize range;

        const VkBufferViewCreateFlags flags = {};
        const void*                   next  = {};
    };

    struct BufferView final
    {
        const VkBufferView handle;
    };

    VkBufferViewCreateInfo
    getBufferViewCreateInfo(const VkBuffer                buffer,
                            const VkFormat                format,
                            const VkDeviceSize            offset,
                            const VkDeviceSize            range,
                            const VkBufferViewCreateFlags flags = {},
                            const void*                   next  = {}) noexcept;

    VkBufferView
    getBufferViewHandle(const VkBufferViewCreateInfo& createInfo, const VkDevice device);

    BufferView
    getBufferView(const BufferViewConfiguration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
