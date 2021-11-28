#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class DescriptorSet final
    {
    public:
        using Handle       = VkDescriptorSet;
        using Handles      = std::vector<Handle>;
        using AllocateInfo = VkDescriptorSetAllocateInfo;
        using Layout       = VkDescriptorSetLayout;
        using Layouts      = std::vector<Layout>;

        struct Configuration final
        {
            const Layouts&         layouts;
            const VkDescriptorPool descriptorPool;
        };

        DescriptorSet() noexcept;
        DescriptorSet(const VkDevice device, const AllocateInfo& allocateInfo);

        DescriptorSet(const DescriptorSet& descriptorSet) = delete;
        DescriptorSet(DescriptorSet&& descriptorSet) noexcept;

        DescriptorSet&
        operator=(const DescriptorSet& descriptorSet) = delete;
        DescriptorSet&
        operator=(DescriptorSet&& descriptorSet) noexcept;

        ~DescriptorSet();

        constexpr const Handles&
        get() const noexcept;

    private:
        VkDevice         device_ {VK_NULL_HANDLE};
        VkDescriptorPool descriptorPool_ {VK_NULL_HANDLE};

        Handles descriptorSets_ {VK_NULL_HANDLE};
    };

    constexpr const DescriptorSet::Handles&
    DescriptorSet::get() const noexcept
    {
        return descriptorSets_;
    }

    DescriptorSet::AllocateInfo
    getDescriptorSetAllocateInfo(const VkDescriptorPool       descriptorPool,
                                 const uint32_t               setLayoutsCount,
                                 const VkDescriptorSetLayout* setLayouts) noexcept;

    DescriptorSet
    getDescriptorSet(const DescriptorSet::Configuration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
