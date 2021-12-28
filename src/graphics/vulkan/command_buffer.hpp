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

    struct CommandBuffers final
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

    CommandBuffers
    getCommandBuffers(const CommandBufferConfiguration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
