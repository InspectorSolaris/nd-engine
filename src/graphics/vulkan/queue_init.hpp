#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    VkQueue
    getQueue(const VkDevice, const u32, const u32) noexcept;

    QueueFamily
    getQueueFamily(opt<const QueueFamilyCfg>::ref, const vec<QueueFamily>&) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
