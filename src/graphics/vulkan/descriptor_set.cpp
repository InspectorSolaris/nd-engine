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

    std::vector<VkDescriptorSet>
    getDescriptorSetHandle(const VkDescriptorSetAllocateInfo& allocateInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        auto descriptorSets = std::vector<VkDescriptorSet>(allocateInfo.descriptorSetCount);

        ND_ASSERT_EXEC(vkAllocateDescriptorSets(device, &allocateInfo, descriptorSets.data()) == VK_SUCCESS);

        return descriptorSets;
    }

    std::vector<VkDescriptorSet>
    getDescriptorSet(const DescriptorSetConfiguration& configuration, const VkDevice device, const void* next)
    {
        ND_SET_SCOPE();

        const auto allocateInfo = getDescriptorSetAllocateInfo(configuration.descriptorPool,
                                                               configuration.layouts.size(),
                                                               configuration.layouts.data(),
                                                               next);

        return getDescriptorSetHandle(allocateInfo, device);
    }
} // namespace nd::src::graphics::vulkan
