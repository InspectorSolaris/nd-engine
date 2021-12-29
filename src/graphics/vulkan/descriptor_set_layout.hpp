#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    struct DescriptorSetLayoutConfiguration final
    {
        const std::vector<VkDescriptorSetLayoutBinding> bindings;

        const VkDescriptorSetLayoutCreateFlags flags = {};
        const void*                            next  = {};
    };

    struct DescriptorSetLayout final
    {
        const VkDescriptorSetLayout handle;
    };

    VkDescriptorSetLayoutCreateInfo
    getDescriptorSetLayoutCreateInfo(const uint32_t                         bindingsCount,
                                     const VkDescriptorSetLayoutBinding*    bindings,
                                     const VkDescriptorSetLayoutCreateFlags flags = {},
                                     const void*                            next  = {}) noexcept;

    VkDescriptorSetLayout
    getDescriptorSetLayoutHandle(const VkDescriptorSetLayoutCreateInfo& createInfo, const VkDevice device);

    DescriptorSetLayout
    getDescriptorSetLayout(const DescriptorSetLayoutConfiguration& configuration, const VkDevice device);

    std::vector<DescriptorSetLayout>
    getDescriptorSetLayouts(const std::vector<DescriptorSetLayoutConfiguration>& configurations, const VkDevice device);
} // namespace nd::src::graphics::vulkan
