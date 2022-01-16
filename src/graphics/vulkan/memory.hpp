#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct DeviceMemoryConfiguration final
    {
        const VkDeviceSize allocationSize;
        const VkDeviceSize alignment;

        const uint32_t memoryTypeIndex;

        const void* next = {};
    };

    struct DeviceMemory final
    {
        const VkDeviceMemory handle;

        const VkDeviceSize size;
        const VkDeviceSize alignment;

        const uint32_t memoryTypeIndex;
    };

    using DeviceMemories = std::vector<DeviceMemory>;

    VkDeviceSize
    getAlignedOffset(const VkDeviceSize offset, const VkDeviceSize alignment) noexcept;

    void
    setMemory(const VkDevice device, const DeviceMemory memory, const VkDeviceSize offset, const void* data) noexcept;

    void
    bindMemories(const VkDevice                          device,
                 const std::vector<Buffer>&              buffers,
                 const std::vector<DeviceMemories>&      bufferMemories,
                 const VkPhysicalDeviceMemoryProperties& physicalDeviceMemoryProperties) noexcept;

    VkMemoryRequirements
    getMemoryRequirements(const VkDevice device, const VkBuffer buffer) noexcept;

    uint32_t
    getMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties& memoryProperties,
                       const VkMemoryRequirements              memoryRequirements,
                       const VkMemoryPropertyFlags             flags);

    VkMemoryAllocateInfo
    getMemoryAllocateInfo(const VkDeviceSize allocationSize, const uint32_t memoryTypeIndex, const void* next = {}) noexcept;

    VkDeviceMemory
    getDeviceMemoryHandle(const VkMemoryAllocateInfo& allocateInfo, const VkDevice device);

    DeviceMemory
    getDeviceMemory(const DeviceMemoryConfiguration& configuration, const VkDevice device);

    std::vector<DeviceMemory>
    getDeviceMemories(const std::vector<DeviceMemoryConfiguration>& configurations, const VkDevice device);

    std::vector<DeviceMemories>
    getAllDeviceMemories(const std::vector<std::vector<DeviceMemoryConfiguration>>& configurations, const VkDevice device);
} // namespace nd::src::graphics::vulkan
