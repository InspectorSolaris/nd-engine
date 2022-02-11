#pragma once

#include "pch.hpp"
#include "tools.hpp"

#include "shared_init.hpp"

#include "window_init.hpp"

namespace nd::src::graphics::glfw
{
    vec<str>
    getGlfwRequiredExtensions() noexcept;

    VkSurfaceKHR
    createSurface(const GlfwWindow, const VkInstance);
} // namespace nd::src::graphics::glfw
