#pragma once

#include "pch.hpp"

namespace nd::src::graphics::glfw
{
    using GlfwWindow = GLFWwindow*;

    class Window final
    {
    public:
        Window() noexcept;
        Window(int width, int height, const char* title) noexcept;

        Window(const Window& window) = delete;
        Window(Window&& window) noexcept;

        Window&
        operator=(const Window& window) = delete;
        Window&
        operator=(Window&& window) noexcept;

        ~Window();

        constexpr GlfwWindow
        get() const noexcept;

        constexpr int
        getWidth() const noexcept;

        constexpr int
        getHeight() const noexcept;

        VkSurfaceKHR
        getSurface(const VkInstance instance) const;

    private:
        GlfwWindow window_;

        int width_;
        int height_;
    };

    constexpr GlfwWindow
    Window::get() const noexcept
    {
        return window_;
    }

    constexpr int
    Window::getWidth() const noexcept
    {
        return width_;
    }

    constexpr int
    Window::getHeight() const noexcept
    {
        return height_;
    }
} // namespace nd::src::graphics::glfw
