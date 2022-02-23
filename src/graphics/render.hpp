#pragma once

// nd::src::graphics::vulkan

#include "objects_complete.hpp"

// nd::src::graphics

#include "scene.hpp"

namespace nd::src::graphics
{
    void
    draw(vulkan::Objects&, const f64) noexcept(ND_VK_ASSERT_NOTHROW&& ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics
