#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    class DescriptorSet final
    {
    public:
        using AllocateInfo = VkDescriptorSetAllocateInfo;

        DescriptorSet() noexcept;
        DescriptorSet(const VkDevice device, const VkDescriptorPool descriptorPool, const AllocateInfo& allocateInfo);

        DescriptorSet(const DescriptorSet& descriptorSet) = delete;
        DescriptorSet(DescriptorSet&& descriptorSet) noexcept;

        DescriptorSet&
        operator=(const DescriptorSet& descriptorSet) = delete;
        DescriptorSet&
        operator=(DescriptorSet&& descriptorSet) noexcept;

        ~DescriptorSet();

        constexpr VkDescriptorSet
        get() const noexcept;

    private:
        VkDevice         device_ {VK_NULL_HANDLE};
        VkDescriptorPool descriptorPool_ {VK_NULL_HANDLE};
        VkDescriptorSet  descriptorSet_ {VK_NULL_HANDLE};
    };

    constexpr VkDescriptorSet
    DescriptorSet::get() const noexcept
    {
        return descriptorSet_;
    }

    DescriptorSet::AllocateInfo
    getDescriptorSetAllocateInfo(const VkDescriptorPool       descriptorPool,
                                 const uint32_t               setLayoutsCount,
                                 const VkDescriptorSetLayout* setLayouts) noexcept;
} // namespace nd::src::graphics::vulkan
