#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    ImageView
    createImageView(opt<const ImageViewCfg>::ref, const VkDevice, const VkImage) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
