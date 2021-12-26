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

    VkDescriptorPool
    getDescriptorPoolHandle(const VkDescriptorPoolCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkDescriptorPool descriptorPool;

        ND_ASSERT_EXEC(vkCreateDescriptorPool(device, &createInfo, nullptr, &descriptorPool) == VK_SUCCESS);

        return descriptorPool;
    }

    VkDescriptorPool
    getDescriptorPool(const DescriptorPoolConfiguration& configuration,
                      const VkDevice                     device,
                      const VkDescriptorPoolCreateFlags  flags,
                      const void*                        next)
    {
        ND_SET_SCOPE();

        const auto createInfo = getDescriptorPoolCreateInfo(configuration.maxSets,
                                                            configuration.descriptorPoolSizes.size(),
                                                            configuration.descriptorPoolSizes.data(),
                                                            flags,
                                                            next);

        return getDescriptorPoolHandle(createInfo, device);
    }
} // namespace nd::src::graphics::vulkan
