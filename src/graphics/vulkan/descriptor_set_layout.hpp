#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    class DescriptorSetLayout final
    {
    public:
        using CreateInfo = VkDescriptorSetLayoutCreateInfo;

        DescriptorSetLayout() noexcept;
        DescriptorSetLayout(const VkDevice device, const CreateInfo &createInfo);

        DescriptorSetLayout(const DescriptorSetLayout &descriptorSetLayout) = delete;
        DescriptorSetLayout(DescriptorSetLayout &&descriptorSetLayout) noexcept;

        DescriptorSetLayout &
        operator=(const DescriptorSetLayout &descriptorSetLayout) = delete;
        DescriptorSetLayout &
        operator=(DescriptorSetLayout &&descriptorSetLayout) noexcept;

        ~DescriptorSetLayout();

        constexpr VkDescriptorSetLayout
        get() const noexcept;

    private:
        VkDevice              device_ {VK_NULL_HANDLE};
        VkDescriptorSetLayout descriptorSetLayout_ {VK_NULL_HANDLE};
    };

    constexpr VkDescriptorSetLayout
    DescriptorSetLayout::get() const noexcept
    {
        return descriptorSetLayout_;
    }

    DescriptorSetLayout::CreateInfo
    getDescriptorSetLayoutCreateInfo(const uint32_t                      bindingsCount,
                                     const VkDescriptorSetLayoutBinding *bindings) noexcept;
}    // namespace nd::src::graphics::vulkan
