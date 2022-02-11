#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    ImageView
    createImageView(opt<const ImageViewCfg>::ref, const VkDevice, const VkImage) noexcept(ND_VULKAN_ASSERT_EXEC_NOTHROW);

    Framebuffer
    createFramebuffer(opt<const FramebufferCfg>::ref, const VkDevice, const vec<VkImageView>&) noexcept(ND_VULKAN_ASSERT_EXEC_NOTHROW);
} // namespace nd::src::graphics::vulkan
