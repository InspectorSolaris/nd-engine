#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    RenderPass
    createRenderPass(opt<const RenderPassCfg>::ref, const VkDevice) noexcept(ND_VULKAN_ASSERT_EXEC_NOTHROW);
} // namespace nd::src::graphics::vulkan
