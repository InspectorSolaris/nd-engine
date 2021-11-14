#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    class DescriptorPool final
    {
    public:
        using CreateInfo = VkDescriptorPoolCreateInfo;

        DescriptorPool() noexcept;
        DescriptorPool(const VkDevice device, const CreateInfo& createInfo);

        DescriptorPool(const DescriptorPool& descriptorPool) = delete;
        DescriptorPool(DescriptorPool&& descriptorPool) noexcept;

        DescriptorPool&
        operator=(const DescriptorPool& descriptorPool) = delete;
        DescriptorPool&
        operator=(DescriptorPool&& descriptorPool) noexcept;

        ~DescriptorPool();

        constexpr VkDescriptorPool
        get() const noexcept;

    private:
        VkDevice         device_ {VK_NULL_HANDLE};
        VkDescriptorPool descriptorPool_ {VK_NULL_HANDLE};
    };

    DescriptorPool::CreateInfo
    getDescriptorPoolCreateInfo(const uint32_t              maxSets,
                                const uint32_t              poolSizesCount,
                                const VkDescriptorPoolSize* poolSizes) noexcept;
} // namespace nd::src::graphics::vulkan
