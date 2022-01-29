#pragma once

#include "pch.hpp"
#include "window.hpp"

namespace nd::src::graphics::glfw
{
    std::vector<std::string>
    getRequiredExtensions() noexcept;

    VkSurfaceKHR
    getSurface(const GlfwWindow window, const VkInstance instance);
} // namespace nd::src::graphics::glfw
