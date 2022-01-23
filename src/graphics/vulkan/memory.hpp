#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct DeviceMemoryConfiguration final
    {
        const VkDeviceSize size;

        const uint32_t memoryTypeIndex;

        const void* next = {};
    };

    struct DeviceMemory final
    {
        const VkDeviceMemory handle;
        const VkDeviceSize   size;

        const uint32_t memoryTypeIndex;
    };

    using DeviceMemories = std::vector<DeviceMemory>;

    VkDeviceSize
    getAlignedOffset(const VkDeviceSize offset, const VkDeviceSize alignment) noexcept;

    void
    setMemory(const VkDevice device, const DeviceMemory memory, const VkDeviceSize offset, const VkDeviceSize size, const void* data) noexcept;

    std::vector<size_t>
    bindBufferMemory(const VkDevice                          device,
                     const VkPhysicalDeviceMemoryProperties* memoryProperties,
                     const std::vector<DeviceMemory>&        deviceMemories,
                     const std::vector<Buffer>&              buffers);

    std::vector<size_t>
    bindImageMemory(const VkDevice                          device,
                    const VkPhysicalDeviceMemoryProperties* memoryProperties,
                    const std::vector<DeviceMemory>&        deviceMemories,
                    const std::vector<Image>&               images);

    VkMemoryRequirements
    getMemoryRequirements(const VkDevice device, const VkBuffer buffer) noexcept;

    VkMemoryRequirements
    getMemoryRequirements(const VkDevice device, const VkImage image) noexcept;

    uint32_t
    getMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties* memoryProperties,
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
