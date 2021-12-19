#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct DescriptorSetConfiguration final
    {
        const std::vector<VkDescriptorSetLayout>& layouts;
        const VkDescriptorPool                    descriptorPool;
    };

    VkDescriptorSetAllocateInfo
    getDescriptorSetAllocateInfo(const VkDescriptorPool       descriptorPool,
                                 const uint32_t               setLayoutsCount,
                                 const VkDescriptorSetLayout* setLayouts,
                                 const void*                  next = {}) noexcept;

    std::vector<VkDescriptorSet>
    getDescriptorSet(const VkDescriptorSetAllocateInfo& allocateInfo, const VkDevice device);

    std::vector<VkDescriptorSet>
    getDescriptorSet(const DescriptorSetConfiguration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
