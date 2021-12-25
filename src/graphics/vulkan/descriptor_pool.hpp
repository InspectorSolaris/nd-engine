#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct DescriptorPoolConfiguration final
    {
        const std::vector<VkDescriptorPoolSize>& descriptorPoolSizes;
        const uint32_t                           maxSets;
    };


    VkDescriptorPoolCreateInfo
    getDescriptorPoolCreateInfo(const uint32_t                    maxSets,
                                const uint32_t                    poolSizesCount,
                                const VkDescriptorPoolSize*       poolSizes,
                                const VkDescriptorPoolCreateFlags flags = {},
                                const void*                       next  = {}) noexcept;

    VkDescriptorPool
    getDescriptorPoolHandle(const VkDescriptorPoolCreateInfo& createInfo, const VkDevice device);

    VkDescriptorPool
    getDescriptorPool(const DescriptorPoolConfiguration& configuration,
                      const VkDevice                     device,
                      const VkDescriptorPoolCreateFlags  flags = {},
                      const void*                        next  = {});
} // namespace nd::src::graphics::vulkan
