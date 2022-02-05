#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    Framebuffer
    createFramebuffer(opt<const FramebufferCfg>::ref, const VkDevice, const vec<VkImageView>&) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
