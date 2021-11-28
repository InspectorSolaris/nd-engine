#include "descriptor_set.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    DescriptorSet::DescriptorSet() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    DescriptorSet::DescriptorSet(const VkDevice device, const AllocateInfo& allocateInfo)
        : device_(device)
        , descriptorPool_(allocateInfo.descriptorPool)
        , descriptorSets_(allocateInfo.descriptorSetCount)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkAllocateDescriptorSets(device_, &allocateInfo, descriptorSets_.data()) == VK_SUCCESS);
    }

    DescriptorSet::DescriptorSet(DescriptorSet&& descriptorSet) noexcept
        : device_(std::move(descriptorSet.device_))
        , descriptorPool_(std::move(descriptorSet.descriptorPool_))
        , descriptorSets_(std::move(descriptorSet.descriptorSets_))
    {
        ND_SET_SCOPE_LOW();

        descriptorSet.descriptorSets_ = Handles(descriptorSets_.size(), VK_NULL_HANDLE);
    }

    DescriptorSet&
    DescriptorSet::operator=(DescriptorSet&& descriptorSet) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&descriptorSet == this)
        {
            return *this;
        }

        device_         = std::move(descriptorSet.device_);
        descriptorPool_ = std::move(descriptorSet.descriptorPool_);
        descriptorSets_ = std::move(descriptorSet.descriptorSets_);

        descriptorSet.descriptorSets_ = Handles(descriptorSets_.size(), VK_NULL_HANDLE);

        return *this;
    }

    DescriptorSet::~DescriptorSet()
    {
        ND_SET_SCOPE_LOW();

        vkFreeDescriptorSets(device_, descriptorPool_, descriptorSets_.size(), descriptorSets_.data());
    }

    DescriptorSet::AllocateInfo
    getDescriptorSetAllocateInfo(const VkDescriptorPool       descriptorPool,
                                 const uint32_t               setLayoutsCount,
                                 const VkDescriptorSetLayout* setLayouts) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, // sType;
            nullptr,                                        // pNext;
            descriptorPool,                                 // descriptorPool;
            setLayoutsCount,                                // descriptorSetCount;
            setLayouts                                      // pSetLayouts;
        };
    }

    DescriptorSet
    getDescriptorSet(const DescriptorSet::Configuration& configuration, const VkDevice device)
    {
        ND_SET_SCOPE_LOW();

        const auto allocateInfo = getDescriptorSetAllocateInfo(configuration.descriptorPool,
                                                               configuration.layouts.size(),
                                                               configuration.layouts.data());

        return DescriptorSet(device, allocateInfo);
    }
} // namespace nd::src::graphics::vulkan
