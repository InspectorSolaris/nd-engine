#pragma once

#include "pch.hpp"
#include "tools.hpp"

#include "objects_cfg_builder.hpp"
#include "objects_init_builder.hpp"

#include "sync_init.hpp"

namespace nd::src::graphics::vulkan
{
    Objects createObjects(opt<const Dependency>::ref, opt<const ObjectsCfg>::ref, opt<const ObjectsInit>::ref) noexcept;

    void destroyObjects(opt<const Objects>::ref) noexcept;
} // namespace nd::src::graphics::vulkan
