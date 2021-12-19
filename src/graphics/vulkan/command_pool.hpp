#pragma once

#include "pch.hpp"
#include "shared.hpp"

#include "queue.hpp"

namespace nd::src::graphics::vulkan
{
    struct CommandPoolConfiguration final
    {
        const std::vector<QueueFamily>& queueFamiliesPool;
        const VkQueueFlags              queueFlags;
    };

    VkCommandPoolCreateInfo
    getCommandPoolCreateInfo(const uint32_t                 queueFamilyIndex,
                             const VkCommandPoolCreateFlags flags = {},
                             const void*                    next  = {}) noexcept;

    VkCommandPool
    getCommandPool(const VkCommandPoolCreateInfo& createInfo, const VkDevice device);

    VkCommandPool
    getCommandPool(const CommandPoolConfiguration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
