#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    void
    resetCommandPools(u64, u64, span<const CommandPool>, const VkDevice, const VkCommandPoolResetFlags = {}) noexcept(ND_VK_ASSERT_NOTHROW);

    CommandPool
    createCommandPool(opt<const CommandPoolCfg>::ref, const VkDevice) noexcept(ND_VK_ASSERT_NOTHROW);

    vec<CommandPool>
    createCommandPools(const vec<CommandPoolCfg>&, const VkDevice) noexcept(ND_VK_ASSERT_NOTHROW);

    CommandPoolObjects
    createCommandPoolObjects(opt<const CommandPoolObjectsCfg>::ref, const VkDevice) noexcept(ND_VK_ASSERT_NOTHROW);

    vec<CommandBuffer>
    allocateCommandBuffers(opt<const CommandBufferCfg>::ref, opt<const CommandPool>::ref, const VkDevice) noexcept(ND_VK_ASSERT_NOTHROW);

    vec<CommandBuffer>
    allocateCommandBuffers(opt<const CommandBufferCfg>::ref, const vec<CommandPool>&, const VkDevice) noexcept(ND_VK_ASSERT_NOTHROW);

    CommandBufferObjects
    allocateCommandBufferObjects(VulkanObjects&,
                                 opt<const CommandBufferObjectsCfg>::ref,
                                 opt<const CommandPoolObjects>::ref,
                                 const VkDevice) noexcept(ND_VK_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
