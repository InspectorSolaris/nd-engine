#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    RenderPass
    createRenderPass(opt<const RenderPassCfg>::ref, const VkDevice) noexcept(ND_VK_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
