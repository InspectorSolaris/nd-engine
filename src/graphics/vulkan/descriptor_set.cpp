#include "descriptor_set.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

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
    getDescriptorSetHandles(const VkDescriptorSetAllocateInfo& allocateInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        auto descriptorSets = std::vector<VkDescriptorSet>(allocateInfo.descriptorSetCount);

        ND_ASSERT_EXEC(vkAllocateDescriptorSets(device, &allocateInfo, descriptorSets.data()) == VK_SUCCESS);

        return descriptorSets;
    }

    std::vector<DescriptorSet>
    getDescriptorSets(const DescriptorSetConfiguration& configuration, const VkDevice device)
    {
        ND_SET_SCOPE();

        const auto allocateInfo = getDescriptorSetAllocateInfo(configuration.descriptorPool,
                                                               configuration.layouts.size(),
                                                               configuration.layouts.data(),
                                                               configuration.next);

        return getDescriptorSetHandles(allocateInfo, device);
    }
} // namespace nd::src::graphics::vulkan
