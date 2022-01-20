#pragma once

#include "pch.hpp"

#include "queue.hpp"

namespace nd::src::graphics::vulkan
{
    struct BufferConfiguration final
    {
        const std::vector<uint32_t> queueFamiliesIndices;

        const VkDeviceSize       size;
        const VkBufferUsageFlags usage;

        const VkMemoryPropertyFlags memoryPropertyFlags;
        const VkMemoryPropertyFlags memoryPropertyNotFlags;

        const VkBufferCreateFlags flags = {};
        const void*               next  = {};
    };

    struct Buffer final
    {
        const VkBuffer handle;

        const VkMemoryPropertyFlags memoryPropertyFlags;
        const VkMemoryPropertyFlags memoryPropertyNotFlags;
    };

    VkBufferCreateInfo
    getBufferCreateInfo(const VkDeviceSize        size,
                        const VkBufferUsageFlags  usage,
                        const VkSharingMode       sharingMode,
                        const uint32_t            queueFamilyIndicesCount,
                        const uint32_t*           queueFamilyIndices,
                        const VkBufferCreateFlags flags = {},
                        const void*               next  = {}) noexcept;

    VkBuffer
    getBufferHandle(const VkBufferCreateInfo& createInfo, const VkDevice device);

    Buffer
    getBuffer(const BufferConfiguration& configuration, const VkDevice device);

    std::vector<Buffer>
    getBuffers(const std::vector<BufferConfiguration>& configurations, const VkDevice device);
} // namespace nd::src::graphics::vulkan
