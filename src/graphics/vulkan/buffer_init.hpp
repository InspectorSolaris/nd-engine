#pragma once

#include "shared_init.hpp"

#include "memory_init.hpp"

namespace nd::src::graphics::vulkan
{
    Buffer
    createBuffer(opt<const BufferCfg>::ref, const VkDevice, const VkPhysicalDevice) noexcept(ND_VK_ASSERT_NOTHROW);

    BufferObjects
    createBufferObjects(opt<const BufferObjectsCfg>::ref, const VkDevice, const VkPhysicalDevice) noexcept(ND_VK_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
