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
    createSemaphore(VulkanObjects&, opt<const SemaphoreCfg>::ref) noexcept(ND_VULKAN_ALLOCATION_CALLBACKS);

    vec<Semaphore>
    createSemaphores(VulkanObjects&, opt<const SemaphoreCfg>::ref, const u16) noexcept(ND_VULKAN_ALLOCATION_CALLBACKS);

    Fence
    createFence(VulkanObjects&, opt<const FenceCfg>::ref) noexcept(ND_VULKAN_ALLOCATION_CALLBACKS);

    vec<Fence>
    createFences(VulkanObjects&, opt<const FenceCfg>::ref, const u16) noexcept(ND_VULKAN_ALLOCATION_CALLBACKS);
} // namespace nd::src::graphics::vulkan
