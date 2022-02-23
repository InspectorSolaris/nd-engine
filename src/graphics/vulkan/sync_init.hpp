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
    createSemaphore(Objects&, opt<const SemaphoreCfg>::ref) noexcept(ND_VK_ASSERT_NOTHROW);

    vec<Semaphore>
    createSemaphores(Objects&, opt<const SemaphoreCfg>::ref, const u16) noexcept(ND_VK_ASSERT_NOTHROW);

    Fence
    createFence(Objects&, opt<const FenceCfg>::ref) noexcept(ND_VK_ASSERT_NOTHROW);

    vec<Fence>
    createFences(Objects&, opt<const FenceCfg>::ref, const u16) noexcept(ND_VK_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
