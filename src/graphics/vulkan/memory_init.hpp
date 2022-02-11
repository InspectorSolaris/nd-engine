#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    VkPhysicalDeviceMemoryProperties
    getMemoryProperties(const VkPhysicalDevice) noexcept;

    VkMemoryRequirements
    getBufferMemoryRequirements(const VkBuffer, const VkDevice) noexcept;

    VkMemoryRequirements
    getImageMemoryRequirements(const VkImage, const VkDevice) noexcept;

    u8
    getMemoryTypeIndex(opt<const DeviceMemoryCfg>::ref, const VkPhysicalDeviceMemoryProperties&) noexcept(ND_ASSERT_NOTHROW);

    VkDeviceSize
    getMemoryOffsetAligned(const VkDeviceSize, const VkDeviceSize) noexcept;

    VkDeviceSize
    getMemoryOffset(opt<const DeviceMemory>::ref memory,
                    const VkMemoryRequirements&  requirements,
                    const VkPhysicalDevice       physicalDevice) noexcept(ND_ASSERT_NOTHROW);

    DeviceMemory
    allocateMemory(opt<const DeviceMemoryCfg>::ref, const VkPhysicalDeviceMemoryProperties&, const VkDevice) noexcept(ND_VK_ASSERT_NOTHROW);

    VkDeviceSize
    bindBufferMemory(const VkBuffer, opt<const DeviceMemory>::ref, const VkDevice, const VkPhysicalDevice) noexcept(ND_VK_ASSERT_NOTHROW);

    VkDeviceSize
    bindImageMemory(const VkImage, opt<const DeviceMemory>::ref, const VkDevice, const VkPhysicalDevice) noexcept(ND_VK_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
