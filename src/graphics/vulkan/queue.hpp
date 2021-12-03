#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct QueueFamily final
    {
        const uint32_t     index {};
        const uint32_t     queueCount {};
        const VkQueueFlags queueFlags {};
    };

    std::vector<uint32_t>
    getQueueFamiliesIndices(const std::vector<const QueueFamily*>& queueFamilies) noexcept;
} // namespace nd::src::graphics::vulkan
