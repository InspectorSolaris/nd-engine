#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct PipelineLayoutConfiguration final
    {
        const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts;
        const std::vector<VkPushConstantRange>&   pushConstantRanges;
    };

    VkPipelineLayoutCreateInfo
    getPipelineLayoutCreateInfo(const uint32_t                    setLayoutsCount,
                                const uint32_t                    pushConstantRangesCount,
                                const VkDescriptorSetLayout*      setLayouts,
                                const VkPushConstantRange*        pushConstantRanges,
                                const VkPipelineLayoutCreateFlags flags = {},
                                const void*                       next  = {}) noexcept;

    VkPipelineLayout
    getPipelineLayout(const VkPipelineLayoutCreateInfo& createInfo, const VkDevice device);

    VkPipelineLayout
    getPipelineLayout(const PipelineLayoutConfiguration& configuration,
                      const VkDevice                     device,
                      const VkPipelineLayoutCreateFlags  flags = {},
                      const void*                        next  = {});
} // namespace nd::src::graphics::vulkan
