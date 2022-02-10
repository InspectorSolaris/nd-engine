#pragma once

#include "pch.hpp"
#include "tools.hpp"

#include "window.hpp"

namespace nd::src::graphics::glfw
{
    vec<str>
    getGlfwRequiredExtensions() noexcept;

    VkSurfaceKHR
    createSurface(const GlfwWindow, const VkInstance, const VkAllocationCallbacks*);
} // namespace nd::src::graphics::glfw
