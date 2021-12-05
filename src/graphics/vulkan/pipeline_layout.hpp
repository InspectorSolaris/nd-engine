#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class PipelineLayout final
    {
    public:
        struct Configuration final
        {
            const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts;
            const std::vector<VkPushConstantRange>&   pushConstantRanges;
        };

        PipelineLayout() noexcept;
        PipelineLayout(const VkDevice device, const VkPipelineLayoutCreateInfo& createInfo);

        PipelineLayout(const PipelineLayout& pipelineLayout) = delete;
        PipelineLayout(PipelineLayout&& pipelineLayout) noexcept;

        PipelineLayout&
        operator=(const PipelineLayout& pipelineLayout) = delete;
        PipelineLayout&
        operator=(PipelineLayout&& pipelineLayout) noexcept;

        ~PipelineLayout();

        constexpr VkPipelineLayout
        get() const noexcept;

    private:
        VkDevice         device_ {VK_NULL_HANDLE};
        VkPipelineLayout pipelineLayout_ {VK_NULL_HANDLE};
    };

    constexpr VkPipelineLayout
    PipelineLayout::get() const noexcept
    {
        return pipelineLayout_;
    }

    VkPipelineLayoutCreateInfo
    getPipelineLayoutCreateInfo(const uint32_t               setLayoutsCount,
                                const uint32_t               pushConstantRangesCount,
                                const VkDescriptorSetLayout* setLayouts,
                                const VkPushConstantRange*   pushConstantRanges) noexcept;

    PipelineLayout
    getPipelineLayout(const PipelineLayout::Configuration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
