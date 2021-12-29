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

    struct CommandBuffer final
    {
        const std::vector<VkCommandBuffer> handles;
    };

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
    getCommandBuffersHandles(const VkCommandBufferAllocateInfo& allocateInfo, const VkDevice device);

    CommandBuffer
    getCommandBuffer(const CommandBufferConfiguration& configuration, const VkDevice device);

    std::vector<CommandBuffer>
    getCommandBuffers(const std::vector<CommandBufferConfiguration>& configurations, const VkDevice device);
} // namespace nd::src::graphics::vulkan
