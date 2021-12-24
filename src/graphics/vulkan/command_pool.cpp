#include "command_pool.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VkCommandPoolCreateInfo
    getCommandPoolCreateInfo(const uint32_t                 queueFamilyIndex,
                             const VkCommandPoolCreateFlags flags,
                             const void*                    next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, // sType;
            next,                                       // pNext;
            flags,                                      // flags;
            queueFamilyIndex                            // queueFamilyIndex;
        };
    }

    VkCommandPool
    getCommandPool(const VkCommandPoolCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkCommandPool commandPool;

        ND_ASSERT(vkCreateCommandPool(device, &createInfo, nullptr, &commandPool) == VK_SUCCESS);

        return commandPool;
    }

    VkCommandPool
    getCommandPool(const CommandPoolConfiguration& configuration,
                   const VkDevice                  device,
                   const VkCommandPoolCreateFlags  flags,
                   const void*                     next)
    {
        ND_SET_SCOPE();

        const auto queueFamily = std::find_if(configuration.queueFamiliesPool.begin(),
                                              configuration.queueFamiliesPool.end(),
                                              [&configuration](const auto& queueFamily)
                                              {
                                                  return isSubmask(queueFamily.queueFlags, configuration.queueFlags);
                                              });

        ND_ASSERT(queueFamily != configuration.queueFamiliesPool.end());

        const auto createInfo = getCommandPoolCreateInfo(queueFamily->index, flags, next);

        return getCommandPool(createInfo, device);
    }
} // namespace nd::src::graphics::vulkan
