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

    VkDescriptorSetLayout
    getDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo& createInfo, const VkDevice device);

    VkDescriptorSetLayout
    getDescriptorSetLayout(const DescriptorSetLayoutConfiguration& configuration,
                           const VkDevice                          device,
                           const VkDescriptorSetLayoutCreateFlags  flags = {},
                           const void*                             next  = {});
} // namespace nd::src::graphics::vulkan
