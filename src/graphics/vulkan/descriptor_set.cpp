#include "descriptor_set.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    DescriptorSet::DescriptorSet() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    DescriptorSet::DescriptorSet(const VkDevice         device,
                                 const VkDescriptorPool descriptorPool,
                                 const AllocateInfo &   createInfo) :
        device_(device),
        descriptorPool_(descriptorPool)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkAllocateDescriptorSets(device_, &createInfo, &descriptorSet_) == VK_SUCCESS);
    }

    DescriptorSet::DescriptorSet(DescriptorSet &&descriptorSet) noexcept :
        device_(std::move(descriptorSet.device_)),
        descriptorPool_(std::move(descriptorSet.descriptorPool_)),
        descriptorSet_(std::move(descriptorSet.descriptorSet_))
    {
        ND_SET_SCOPE_LOW();

        descriptorSet.descriptorSet_ = VK_NULL_HANDLE;
    }

    DescriptorSet &
    DescriptorSet::operator=(DescriptorSet &&descriptorSet) noexcept
    {
        ND_SET_SCOPE_LOW();

        if (&descriptorSet == this)
        {
            return *this;
        }

        device_         = std::move(descriptorSet.device_);
        descriptorPool_ = std::move(descriptorSet.descriptorPool_);
        descriptorSet_  = std::move(descriptorSet.descriptorSet_);

        descriptorSet.descriptorSet_ = VK_NULL_HANDLE;

        return *this;
    }

    DescriptorSet::~DescriptorSet()
    {
        ND_SET_SCOPE_LOW();

        vkFreeDescriptorSets(device_, descriptorPool_, 1, &descriptorSet_);
    }

    DescriptorSet::AllocateInfo
    getDescriptorSetAllocateInfo(const VkDescriptorPool       descriptorPool,
                                 const uint32_t               setLayoutsCount,
                                 const VkDescriptorSetLayout *setLayouts) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,    // sType;
            nullptr,                                           // pNext;
            descriptorPool,                                    // descriptorPool;
            setLayoutsCount,                                   // descriptorSetCount;
            setLayouts                                         // pSetLayouts;
        };
    }
}    // namespace nd::src::graphics::vulkan
