#pragma once

#include "pch.hpp"

#include "queue.hpp"

namespace nd::src::graphics::vulkan
{
    struct CommandPoolConfiguration final
    {
        const uint32_t     queueFamilyIndex;
        const uint32_t     queueCount;
        const VkQueueFlags queueFlags;

        const VkCommandPoolCreateFlags flags = {};
        const void*                    next  = {};
    };

    struct CommandPool final
    {
        const uint32_t      queueFamilyIndex;
        const uint32_t      queueCount;
        const VkQueueFlags  queueFlags;
        const VkCommandPool handle;
    };

    VkCommandPoolCreateInfo
    getCommandPoolCreateInfo(const uint32_t queueFamilyIndex, const VkCommandPoolCreateFlags flags = {}, const void* next = {}) noexcept;

    VkCommandPool
    getCommandPoolHandle(const VkCommandPoolCreateInfo& createInfo, const VkDevice device);

    CommandPool
    getCommandPool(const CommandPoolConfiguration& configuration, const VkDevice device);

    std::vector<CommandPool>
    getCommandPools(const std::vector<CommandPoolConfiguration>& configurations, const VkDevice device);
} // namespace nd::src::graphics::vulkan
