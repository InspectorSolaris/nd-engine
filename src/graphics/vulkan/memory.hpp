#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct DeviceMemoryConfiguration final
    {
        const VkDeviceSize allocationSize;
        const VkDeviceSize alignment;
        const uint32_t     memoryTypeIndex;

        const void* next = {};
    };

    struct DeviceMemory final
    {
        const VkDeviceMemory handle;

        const VkDeviceSize size;
        const VkDeviceSize alignment;
    };

    using DeviceMemories = std::vector<DeviceMemory>;

    void
    mapMemory(const VkDevice device, const DeviceMemory memory, const VkDeviceSize offsetMin, const void* data) noexcept;

    VkPhysicalDeviceMemoryProperties
    getPhysicalDeviceMemoryProperties(const VkPhysicalDevice physicalDevice) noexcept;

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
} // namespace nd::src::graphics::vulkan
