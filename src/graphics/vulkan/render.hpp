#pragma once

#include "objects_complete.hpp"

#include "sync_init.hpp"

namespace nd::src::graphics::vulkan
{
    void
    draw(VulkanObjects&, const f64) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
