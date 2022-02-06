#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    CommandPool
    createCommandPool(opt<const CommandPoolCfg>::ref, const VkDevice) noexcept(ND_ASSERT_NOTHROW);

    CommandPoolObjects
    createCommandPoolObjects(opt<const CommandPoolObjectsCfg>::ref, const VkDevice) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
