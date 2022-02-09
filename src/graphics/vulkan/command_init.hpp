#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    void
    resetCommandPools(gsl::span<const CommandPool>, const VkDevice, const VkCommandPoolResetFlags = {}) noexcept;

    CommandPool
    createCommandPool(opt<const CommandPoolCfg>::ref, const VkDevice) noexcept(ND_ASSERT_NOTHROW);

    CommandPoolObjects
    createCommandPoolObjects(opt<const CommandPoolObjectsCfg>::ref, const VkDevice) noexcept(ND_ASSERT_NOTHROW);

    vec<CommandBuffer>
    allocateCommandBuffers(opt<const CommandBufferCfg>::ref, opt<const CommandPool>::ref, const VkDevice) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
