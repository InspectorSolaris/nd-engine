#pragma once

#include "pch.hpp"
#include "tools.hpp"

#include "objects_cfg_builder.hpp"
#include "objects_init_builder.hpp"

#include "sync_init.hpp"

namespace nd::src::graphics::vulkan
{
    VulkanObjects createVulkanObjects(opt<const Dependency>::ref, opt<const VulkanObjectsCfg>::ref, opt<const VulkanObjectsInit>::ref) noexcept;

    void destroyVulkanObjects(opt<const VulkanObjects>::ref) noexcept;
} // namespace nd::src::graphics::vulkan
