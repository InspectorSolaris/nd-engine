#include "descriptor_pool.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VkDescriptorPoolCreateInfo
    getDescriptorPoolCreateInfo(const uint32_t                    maxSets,
                                const uint32_t                    poolSizesCount,
                                const VkDescriptorPoolSize*       poolSizes,
                                const VkDescriptorPoolCreateFlags flags,
                                const void*                       next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO, // sType;
            next,                                          // pNext;
            flags,                                         // flags;
            maxSets,                                       // maxSets;
            poolSizesCount,                                // poolSizeCount;
            poolSizes                                      // pPoolSizes;
        };
    }

    VkDescriptorPoolCreateInfo
    getDescriptorPoolCreateInfo(const DescriptorPoolConfiguration& configuration)
    {
        ND_SET_SCOPE();

        return getDescriptorPoolCreateInfo(configuration.maxSets,
                                           configuration.descriptorPoolSizes.size(),
                                           configuration.descriptorPoolSizes.data(),
                                           configuration.flags);
    }

    VkDescriptorPool
    getDescriptorPool(const VkDescriptorPoolCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkDescriptorPool descriptorPool;

        ND_ASSERT(vkCreateDescriptorPool(device, &createInfo, nullptr, &descriptorPool) == VK_SUCCESS);

        return descriptorPool;
    }
} // namespace nd::src::graphics::vulkan
