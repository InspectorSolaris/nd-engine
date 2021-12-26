#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct DescriptorSetLayoutConfiguration final
    {
        const std::vector<VkDescriptorSetLayoutBinding>& bindings;

        const VkDescriptorSetLayoutCreateFlags flags = {};
        const void*                            next  = {};
    };

    VkDescriptorSetLayoutCreateInfo
    getDescriptorSetLayoutCreateInfo(const uint32_t                         bindingsCount,
                                     const VkDescriptorSetLayoutBinding*    bindings,
                                     const VkDescriptorSetLayoutCreateFlags flags = {},
                                     const void*                            next  = {}) noexcept;

    VkDescriptorSetLayout
    getDescriptorSetLayoutHandle(const VkDescriptorSetLayoutCreateInfo& createInfo, const VkDevice device);

    VkDescriptorSetLayout
    getDescriptorSetLayout(const DescriptorSetLayoutConfiguration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
