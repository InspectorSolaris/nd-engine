#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    class PipelineLayout final
    {
    public:
        using CreateInfo = VkPipelineLayoutCreateInfo;

        PipelineLayout() noexcept;
        PipelineLayout(const VkDevice device, const CreateInfo& createInfo);

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

    PipelineLayout::CreateInfo
    getPipelineLayoutCreateInfo(const uint32_t               setLayoutsCount,
                                const uint32_t               pushConstantRangesCount,
                                const VkDescriptorSetLayout* setLayouts,
                                const VkPushConstantRange*   pushConstantRanges) noexcept;
} // namespace nd::src::graphics::vulkan
