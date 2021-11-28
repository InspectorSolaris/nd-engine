#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class DescriptorPool final
    {
    public:
        using Handle     = VkDescriptorPool;
        using CreateInfo = VkDescriptorPoolCreateInfo;
        using Size       = VkDescriptorPoolSize;
        using Sizes      = std::vector<Size>;

        struct Configuration final
        {
            const Sizes&   descriptorPoolSizes;
            const uint32_t maxSets;
        };

        DescriptorPool() noexcept;
        DescriptorPool(const VkDevice device, const CreateInfo& createInfo);

        DescriptorPool(const DescriptorPool& descriptorPool) = delete;
        DescriptorPool(DescriptorPool&& descriptorPool) noexcept;

        DescriptorPool&
        operator=(const DescriptorPool& descriptorPool) = delete;
        DescriptorPool&
        operator=(DescriptorPool&& descriptorPool) noexcept;

        ~DescriptorPool();

        constexpr Handle
        get() const noexcept;

    private:
        VkDevice         device_ {VK_NULL_HANDLE};
        VkDescriptorPool descriptorPool_ {VK_NULL_HANDLE};
    };

    constexpr DescriptorPool::Handle
    DescriptorPool::get() const noexcept
    {
        return descriptorPool_;
    }

    DescriptorPool::CreateInfo
    getDescriptorPoolCreateInfo(const uint32_t              maxSets,
                                const uint32_t              poolSizesCount,
                                const VkDescriptorPoolSize* poolSizes) noexcept;

    DescriptorPool
    getDescriptorPool(const DescriptorPool::Configuration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
