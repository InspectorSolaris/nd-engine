#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct Surface final
    {
        const VkSurfaceKHR handle;
    };
} // namespace nd::src::graphics::vulkan
