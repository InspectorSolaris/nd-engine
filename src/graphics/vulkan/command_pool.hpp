#pragma once

#include "pch.hpp"
#include "shared.hpp"

#include "queue.hpp"

namespace nd::src::graphics::vulkan
{
    struct CommandPoolConfiguration final
    {
        const uint32_t queueFamilyIndex;

        const VkCommandPoolCreateFlags flags = {};
        const void*                    next  = {};
    };

    struct CommandPool final
    {
        const VkCommandPool handle;
    };

    VkCommandPoolCreateInfo
    getCommandPoolCreateInfo(const uint32_t                 queueFamilyIndex,
                             const VkCommandPoolCreateFlags flags = {},
                             const void*                    next  = {}) noexcept;

    VkCommandPool
    getCommandPoolHandle(const VkCommandPoolCreateInfo& createInfo, const VkDevice device);

    CommandPool
    getCommandPool(const CommandPoolConfiguration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
