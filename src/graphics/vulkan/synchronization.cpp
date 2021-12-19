#include "synchronization.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VkSemaphoreCreateInfo
    getSemaphoreCreateInfo(const VkSemaphoreCreateFlags flags, const void* next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, // sType;
            next,                                    // pNext;
            flags                                    // flags;
        };
    }

    VkSemaphore
    getSemaphore(const VkSemaphoreCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkSemaphore semaphore;

        ND_ASSERT(vkCreateSemaphore(device, &createInfo, nullptr, &semaphore) == VK_SUCCESS);

        return semaphore;
    }

    VkSemaphore
    getSemaphore(const VkDevice device)
    {
        ND_SET_SCOPE();

        const auto createInfo = getSemaphoreCreateInfo();

        return getSemaphore(createInfo, device);
    }

    VkFenceCreateInfo
    getFenceCreateInfo(const VkFenceCreateFlags flags, const void* next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, // sType;
            next,                                // pNext;
            flags                                // flags;
        };
    }

    VkFence
    getFence(const VkFenceCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkFence fence;

        ND_ASSERT(vkCreateFence(device, &createInfo, nullptr, &fence) == VK_SUCCESS);

        return fence;
    }

    VkFence
    getFence(const VkDevice device)
    {
        ND_SET_SCOPE();

        const auto createInfo = getFenceCreateInfo();

        return getFence(createInfo, device);
    }
} // namespace nd::src::graphics::vulkan
