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

        ND_ASSERT_EXEC(vkCreateSemaphore(device, &createInfo, nullptr, &semaphore) == VK_SUCCESS);

        return semaphore;
    }

    VkSemaphore
    getSemaphore(const VkDevice device, const VkSemaphoreCreateFlags flags, const void* next)
    {
        ND_SET_SCOPE();

        const auto createInfo = getSemaphoreCreateInfo(flags, next);

        return getSemaphore(createInfo, device);
    }

    std::vector<VkSemaphore>
    getSemaphore(const VkDevice device, const size_t count, const VkSemaphoreCreateFlags flags, const void* next)
    {
        return getMapped<VkSemaphore>(count,
                                      [device, flags, next](const auto index)
                                      {
                                          return getSemaphore(device, flags, next);
                                      });
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

        ND_ASSERT_EXEC(vkCreateFence(device, &createInfo, nullptr, &fence) == VK_SUCCESS);

        return fence;
    }

    VkFence
    getFence(const VkDevice device, const VkSemaphoreCreateFlags flags, const void* next)
    {
        ND_SET_SCOPE();

        const auto createInfo = getFenceCreateInfo(flags, next);

        return getFence(createInfo, device);
    }

    std::vector<VkFence>
    getFence(const VkDevice device, const size_t count, const VkFenceCreateFlags flags, const void* next)
    {
        return getMapped<VkFence>(count,
                                  [device, flags, next](const auto index)
                                  {
                                      return getFence(device, flags, next);
                                  });
    }
} // namespace nd::src::graphics::vulkan
