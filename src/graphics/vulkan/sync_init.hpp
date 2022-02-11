#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    struct SemaphoreCfg final
    {
        void*                  next  = {};
        VkSemaphoreCreateFlags flags = {};
    };

    struct FenceCfg final
    {
        void*              next  = {};
        VkFenceCreateFlags flags = {};
    };

    Semaphore
    createSemaphore(VulkanObjects&, opt<const SemaphoreCfg>::ref) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);

    vec<Semaphore>
    createSemaphores(VulkanObjects&, opt<const SemaphoreCfg>::ref, const u16) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);

    Fence
    createFence(VulkanObjects&, opt<const FenceCfg>::ref) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);

    vec<Fence>
    createFences(VulkanObjects&, opt<const FenceCfg>::ref, const u16) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);
} // namespace nd::src::graphics::vulkan
