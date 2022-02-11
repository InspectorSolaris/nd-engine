#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    vec<VkLayerProperties>
    getInstanceLayerProperties() noexcept(ND_VK_ASSERT_NOTHROW);

    vec<VkExtensionProperties>
    getInstanceExtensionProperties() noexcept(ND_VK_ASSERT_NOTHROW);

    bool
    isInstanceLayersSupported(const vec<str>&) noexcept(ND_VK_ASSERT_NOTHROW);

    bool
    isInstanceExtensionsSupported(const vec<str>&) noexcept(ND_VK_ASSERT_NOTHROW);

    Instance createInstance(opt<const InstanceCfg>::ref) noexcept(ND_VK_ASSERT_NOTHROW&& ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
