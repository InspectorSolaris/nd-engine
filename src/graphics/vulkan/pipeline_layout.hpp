#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct PipelineLayoutConfiguration final
    {
        const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts;
        const std::vector<VkPushConstantRange>&   pushConstantRanges;

        const VkPipelineLayoutCreateFlags flags = {};
        const void*                       next  = {};
    };

    struct PipelineLayout final
    {
        const VkPipelineLayout handle;
    };

    VkPipelineLayoutCreateInfo
    getPipelineLayoutCreateInfo(const uint32_t                    setLayoutsCount,
                                const uint32_t                    pushConstantRangesCount,
                                const VkDescriptorSetLayout*      setLayouts,
                                const VkPushConstantRange*        pushConstantRanges,
                                const VkPipelineLayoutCreateFlags flags = {},
                                const void*                       next  = {}) noexcept;

    VkPipelineLayout
    getPipelineLayoutHandle(const VkPipelineLayoutCreateInfo& createInfo, const VkDevice device);

    PipelineLayout
    getPipelineLayout(const PipelineLayoutConfiguration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
