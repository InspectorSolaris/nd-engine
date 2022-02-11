#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    void
    resetCommandPools(gsl::span<const CommandPool>, const VkDevice, const VkCommandPoolResetFlags = {}) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);

    CommandPool
    createCommandPool(opt<const CommandPoolCfg>::ref, const VkDevice) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);

    vec<CommandPool>
    createCommandPools(const vec<CommandPoolCfg>& cfg, const VkDevice device) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);

    CommandPoolObjects
    createCommandPoolObjects(opt<const CommandPoolObjectsCfg>::ref, const VkDevice) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);

    vec<CommandBuffer>
    allocateCommandBuffers(opt<const CommandBufferCfg>::ref, opt<const CommandPool>::ref, const VkDevice) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);

    vec<CommandBuffer>
    allocateCommandBuffers(opt<const CommandBufferCfg>::ref cfg,
                           const vec<CommandPool>&          commandPools,
                           const VkDevice                   device) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);
} // namespace nd::src::graphics::vulkan
