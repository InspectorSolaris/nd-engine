#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct DeviceMemoryConfiguration final
    {
        const VkDeviceSize allocationSize;
        const uint32_t     memoryTypeIndex;

        const void* next = {};
    };

    using DeviceMemory   = VkDeviceMemory;
    using DeviceMemories = std::vector<VkDeviceMemory>;

    VkPhysicalDeviceMemoryProperties
    getPhysicalDeviceMemoryProperties(const VkPhysicalDevice physicalDevice) noexcept;

    VkMemoryRequirements
    getMemoryRequirements(const VkDevice device, const VkBuffer buffer) noexcept;

    VkMemoryAllocateInfo
    getMemoryAllocateInfo(const VkDeviceSize allocationSize, const uint32_t memoryTypeIndex, const void* next = {}) noexcept;

    VkDeviceMemory
    getDeviceMemoryHandle(const VkMemoryAllocateInfo& allocateInfo, const VkDevice device);

    DeviceMemory
    getDeviceMemory(const DeviceMemoryConfiguration& configuration, const VkDevice device);

    std::vector<DeviceMemory>
    getDeviceMemories(const std::vector<DeviceMemoryConfiguration>& configurations, const VkDevice device);
} // namespace nd::src::graphics::vulkan
