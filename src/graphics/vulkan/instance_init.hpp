#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    vec<VkLayerProperties>
    getInstanceLayerProperties() noexcept;

    vec<VkExtensionProperties>
    getInstanceExtensionProperties() noexcept;

    bool
    isInstanceLayersSupported(const vec<str>&) noexcept;

    bool
    isInstanceExtensionsSupported(const vec<str>&) noexcept;

    Instance createInstance(opt<const InstanceCfg>::ref) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
