#pragma once

#include "objects_complete.hpp"

namespace nd::src::graphics::vulkan
{
    void
    draw(VulkanObjects&, const f64) noexcept(ND_VULKAN_ASSERT_EXEC_NOTHROW&& ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
