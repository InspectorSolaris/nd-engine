#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    struct CommandBufferConfiguration final
    {
        const VkCommandPool        commandPool;
        const VkCommandBufferLevel level;
        const uint32_t             count;

        const void* next = {};
    };

    using CommandBuffers = std::vector<VkCommandBuffer>;

    VkCommandBufferBeginInfo
    getCommandBufferBeginInfo(const VkCommandBufferInheritanceInfo* inheritanceInfo,
                              const VkCommandBufferUsageFlags       flags = {},
                              const void*                           next  = {}) noexcept;

    VkCommandBufferAllocateInfo
    getCommandBufferAllocateInfo(const VkCommandPool        commandPool,
                                 const VkCommandBufferLevel level,
                                 const uint32_t             commandBufferCount,
                                 const void*                next = {}) noexcept;

    std::vector<VkCommandBuffer>
    getCommandBufferHandles(const VkCommandBufferAllocateInfo& allocateInfo, const VkDevice device);

    CommandBuffers
    getCommandBuffer(const CommandBufferConfiguration& configuration, const VkDevice device);

    std::vector<CommandBuffers>
    getCommandBuffers(const std::vector<CommandBufferConfiguration>& configurations, const VkDevice device);
} // namespace nd::src::graphics::vulkan
