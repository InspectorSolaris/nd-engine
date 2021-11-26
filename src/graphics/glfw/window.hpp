#pragma once

#include "pch.hpp"

namespace nd::src::graphics::glfw
{
    using GlfwWindow = GLFWwindow*;

    class Window final
    {
    public:
        struct Configuration final
        {
            const std::string& title;
            const int          width;
            const int          height;
        };

        Window() noexcept;
        Window(const int width, const int height, const std::string& title) noexcept;

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

    private:
        GlfwWindow window_ {};

        int width_ {};
        int height_ {};
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

    Window
    getWindow(const Window::Configuration& configuration) noexcept;
} // namespace nd::src::graphics::glfw
