#pragma once

#include "pch.hpp"
#include "tools.hpp"

namespace nd::src::graphics::glfw
{
    using GlfwWindow = GLFWwindow*;

    struct Window final
    {
        GlfwWindow handle;

        u16 width;
        u16 height;
    };

    struct WindowCfg final
    {
        const str& title;
        const u16  width;
        const u16  height;
    };

    Window
    getWindow(const WindowCfg&) noexcept;
} // namespace nd::src::graphics::glfw
