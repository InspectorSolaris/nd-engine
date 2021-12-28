#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    struct DescriptorSetConfiguration final
    {
        const std::vector<VkDescriptorSetLayout>& layouts;
        const VkDescriptorPool                    descriptorPool;

        const void* next = {};
    };

    struct DescriptorSets final
    {
        const std::vector<VkDescriptorSet> handles;
    };

    VkDescriptorSetAllocateInfo
    getDescriptorSetAllocateInfo(const VkDescriptorPool       descriptorPool,
                                 const uint32_t               setLayoutsCount,
                                 const VkDescriptorSetLayout* setLayouts,
                                 const void*                  next = {}) noexcept;

    std::vector<VkDescriptorSet>
    getDescriptorSetsHandles(const VkDescriptorSetAllocateInfo& allocateInfo, const VkDevice device);

    DescriptorSets
    getDescriptorSets(const DescriptorSetConfiguration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
