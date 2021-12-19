#include "descriptor_set.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VkDescriptorSetAllocateInfo
    getDescriptorSetAllocateInfo(const VkDescriptorPool       descriptorPool,
                                 const uint32_t               setLayoutsCount,
                                 const VkDescriptorSetLayout* setLayouts,
                                 const void*                  next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, // sType;
            next,                                           // pNext;
            descriptorPool,                                 // descriptorPool;
            setLayoutsCount,                                // descriptorSetCount;
            setLayouts                                      // pSetLayouts;
        };
    }

    VkDescriptorSetAllocateInfo
    getDescriptorSetAllocateInfo(const DescriptorSetConfiguration& configuration)
    {
        ND_SET_SCOPE();

        return getDescriptorSetAllocateInfo(configuration.descriptorPool,
                                            configuration.layouts.size(),
                                            configuration.layouts.data());
    }

    std::vector<VkDescriptorSet>
    getDescriptorSet(const VkDescriptorSetAllocateInfo& allocateInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        auto descriptorSets = std::vector<VkDescriptorSet>(allocateInfo.descriptorSetCount);

        ND_ASSERT(vkAllocateDescriptorSets(device, &allocateInfo, descriptorSets.data()) == VK_SUCCESS);

        return descriptorSets;
    }
} // namespace nd::src::graphics::vulkan
