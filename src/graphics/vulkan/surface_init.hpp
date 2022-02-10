#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    Surface
    createSurface(const VkInstance) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
