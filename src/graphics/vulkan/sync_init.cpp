#include "sync_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    Semaphore
    createSemaphore(Objects& objects, opt<const SemaphoreCfg>::ref cfg) noexcept(ND_VK_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto createInfo = VkSemaphoreCreateInfo {.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, .pNext = cfg.next, .flags = cfg.flags};

        VkSemaphore semaphore;

        ND_VK_ASSERT(vkCreateSemaphore(objects.device.handle, &createInfo, ND_VK_ALLOCATION_CALLBACKS, &semaphore));

        objects.semaphores.push_back(semaphore);

        return semaphore;
    }

    vec<Semaphore>
    createSemaphores(Objects& objects, opt<const SemaphoreCfg>::ref cfg, const u16 count) noexcept(ND_VK_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return getMapped<Semaphore>(count,
                                    [&objects, &cfg](const auto index)
                                    {
                                        return createSemaphore(objects, cfg);
                                    });
    }

    Fence
    createFence(Objects& objects, opt<const FenceCfg>::ref cfg) noexcept(ND_VK_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto createInfo = VkFenceCreateInfo {.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .pNext = cfg.next, .flags = cfg.flags};

        VkFence fence;

        ND_VK_ASSERT(vkCreateFence(objects.device.handle, &createInfo, ND_VK_ALLOCATION_CALLBACKS, &fence));

        objects.fences.push_back(fence);

        return fence;
    }

    vec<Fence>
    createFences(Objects& objects, opt<const FenceCfg>::ref cfg, const u16 count) noexcept(ND_VK_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return getMapped<Fence>(count,
                                [&objects, &cfg](const auto index)
                                {
                                    return createFence(objects, cfg);
                                });
    }
} // namespace nd::src::graphics::vulkan
