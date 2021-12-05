#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class DescriptorSet final
    {
    public:
        struct Configuration final
        {
            const std::vector<VkDescriptorSetLayout>& layouts;
            const VkDescriptorPool                    descriptorPool;
        };

        DescriptorSet() noexcept;
        DescriptorSet(const VkDevice device, const VkDescriptorSetAllocateInfo& allocateInfo);

        DescriptorSet(const DescriptorSet& descriptorSet) = delete;
        DescriptorSet(DescriptorSet&& descriptorSet) noexcept;

        DescriptorSet&
        operator=(const DescriptorSet& descriptorSet) = delete;
        DescriptorSet&
        operator=(DescriptorSet&& descriptorSet) noexcept;

        ~DescriptorSet();

        constexpr const std::vector<VkDescriptorSet>&
        get() const noexcept;

    private:
        VkDevice         device_ {VK_NULL_HANDLE};
        VkDescriptorPool descriptorPool_ {VK_NULL_HANDLE};

        std::vector<VkDescriptorSet> descriptorSets_ {VK_NULL_HANDLE};
    };

    constexpr const std::vector<VkDescriptorSet>&
    DescriptorSet::get() const noexcept
    {
        return descriptorSets_;
    }

    VkDescriptorSetAllocateInfo
    getDescriptorSetAllocateInfo(const VkDescriptorPool       descriptorPool,
                                 const uint32_t               setLayoutsCount,
                                 const VkDescriptorSetLayout* setLayouts,
                                 const void*                  next = {}) noexcept;

    DescriptorSet
    getDescriptorSet(const DescriptorSet::Configuration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
