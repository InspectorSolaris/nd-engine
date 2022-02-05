#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    u8
    getMemoryTypeIndex(opt<const DeviceMemoryCfg>::ref, const VkPhysicalDeviceMemoryProperties&) noexcept(ND_ASSERT_NOTHROW);

    DeviceMemory
    allocateMemory(opt<const DeviceMemoryCfg>::ref, const VkPhysicalDeviceMemoryProperties&, const VkDevice) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
