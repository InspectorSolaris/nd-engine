#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class DescriptorSetLayout final
    {
    public:
        struct Configuration final
        {
            const std::vector<VkDescriptorSetLayoutBinding>& bindings;
        };

        DescriptorSetLayout() noexcept;
        DescriptorSetLayout(const VkDevice device, const VkDescriptorSetLayoutCreateInfo& createInfo);

        DescriptorSetLayout(const DescriptorSetLayout& descriptorSetLayout) = delete;
        DescriptorSetLayout(DescriptorSetLayout&& descriptorSetLayout) noexcept;

        DescriptorSetLayout&
        operator=(const DescriptorSetLayout& descriptorSetLayout) = delete;
        DescriptorSetLayout&
        operator=(DescriptorSetLayout&& descriptorSetLayout) noexcept;

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

    VkDescriptorSetLayoutCreateInfo
    getDescriptorSetLayoutCreateInfo(const uint32_t bindingsCount, const VkDescriptorSetLayoutBinding* bindings) noexcept;

    DescriptorSetLayout
    getDescriptorSetLayout(const DescriptorSetLayout::Configuration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
