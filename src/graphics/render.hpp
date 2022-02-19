#pragma once

#include "objects_complete.hpp"

namespace nd::src::graphics::vulkan
{
    void
    draw(VulkanObjects&, const f64) noexcept(ND_VK_ASSERT_NOTHROW&& ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
