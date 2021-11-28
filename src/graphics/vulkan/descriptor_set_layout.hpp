#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class DescriptorSetLayout final
    {
    public:
        using Handle     = VkDescriptorSetLayout;
        using CreateInfo = VkDescriptorSetLayoutCreateInfo;
        using Binding    = VkDescriptorSetLayoutBinding;
        using Bindings   = std::vector<Binding>;

        struct Configuration final
        {
            const Bindings& bindings;
        };

        DescriptorSetLayout() noexcept;
        DescriptorSetLayout(const VkDevice device, const CreateInfo& createInfo);

        DescriptorSetLayout(const DescriptorSetLayout& descriptorSetLayout) = delete;
        DescriptorSetLayout(DescriptorSetLayout&& descriptorSetLayout) noexcept;

        DescriptorSetLayout&
        operator=(const DescriptorSetLayout& descriptorSetLayout) = delete;
        DescriptorSetLayout&
        operator=(DescriptorSetLayout&& descriptorSetLayout) noexcept;

        ~DescriptorSetLayout();

        constexpr Handle
        get() const noexcept;

    private:
        VkDevice device_ {VK_NULL_HANDLE};
        Handle   descriptorSetLayout_ {VK_NULL_HANDLE};
    };

    constexpr DescriptorSetLayout::Handle
    DescriptorSetLayout::get() const noexcept
    {
        return descriptorSetLayout_;
    }

    DescriptorSetLayout::CreateInfo
    getDescriptorSetLayoutCreateInfo(const uint32_t bindingsCount, const VkDescriptorSetLayoutBinding* bindings) noexcept;

    DescriptorSetLayout
    getDescriptorSetLayout(const DescriptorSetLayout::Configuration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
