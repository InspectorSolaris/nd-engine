#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct DescriptorSetLayoutConfiguration final
    {
        const std::vector<VkDescriptorSetLayoutBinding>& bindings;
    };

    VkDescriptorSetLayoutCreateInfo
    getDescriptorSetLayoutCreateInfo(const uint32_t                         bindingsCount,
                                     const VkDescriptorSetLayoutBinding*    bindings,
                                     const VkDescriptorSetLayoutCreateFlags flags = {},
                                     const void*                            next  = {}) noexcept;

    VkDescriptorSetLayoutCreateInfo
    getDescriptorSetLayoutCreateInfo(const DescriptorSetLayoutConfiguration& configuration);

    VkDescriptorSetLayout
    getDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo& createInfo, const VkDevice device);
} // namespace nd::src::graphics::vulkan
