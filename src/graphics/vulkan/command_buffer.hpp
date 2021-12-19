#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct CommandBufferConfiguration final
    {
        const VkCommandPool        commandPool;
        const VkCommandBufferLevel level;
        const uint32_t             count;
    };

    VkCommandBufferAllocateInfo
    getCommandBufferAllocateInfo(const VkCommandPool        commandPool,
                                 const VkCommandBufferLevel level,
                                 const uint32_t             commandBufferCount,
                                 const void*                next = {}) noexcept;

    std::vector<VkCommandBuffer>
    getCommandBuffer(const VkCommandBufferAllocateInfo& allocateInfo, const VkDevice device);

    std::vector<VkCommandBuffer>
    getCommandBuffer(const CommandBufferConfiguration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
