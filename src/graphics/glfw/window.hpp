#pragma once

#include "pch.hpp"

namespace nd::src::graphics::glfw
{
    using GlfwWindow = GLFWwindow*;

    struct WindowConfiguration final
    {
        const std::string& title;
        const int          width;
        const int          height;
    };

    GlfwWindow
    getWindow(const WindowConfiguration& configuration) noexcept;
} // namespace nd::src::graphics::glfw
