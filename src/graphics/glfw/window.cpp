#include "window.hpp"
#include "tools.hpp"

namespace nd::src::graphics::glfw
{
    Window::Window() noexcept
    {
        ND_SET_SCOPE();
    }

    Window::Window(const int width, const int height, const std::string& title) noexcept
        : width_(width)
        , height_(height)
    {
        ND_SET_SCOPE();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window_ = glfwCreateWindow(width_, height_, title.c_str(), nullptr, nullptr);
    }

    Window::Window(Window&& window) noexcept
        : window_(std::move(window.window_))
        , width_(std::move(window.width_))
        , height_(std::move(window.height_))
    {
        ND_SET_SCOPE();

        window.window_ = nullptr;
        window.width_  = 0;
        window.height_ = 0;
    }

    Window&
    Window::operator=(Window&& window) noexcept
    {
        ND_SET_SCOPE();

        if(&window == this)
        {
            return *this;
        }

        window_ = std::move(window.window_);
        width_  = std::move(window.width_);
        height_ = std::move(window.height_);

        window.window_ = nullptr;
        window.width_  = 0;
        window.height_ = 0;

        return *this;
    }

    Window::~Window()
    {
        ND_SET_SCOPE();

        if(window_ != nullptr)
        {
            glfwDestroyWindow(window_);
        }
    }

    Window
    getWindow(const Window::Configuration& configuration) noexcept
    {
        ND_SET_SCOPE();

        return Window(configuration.width, configuration.height, configuration.title);
    }
} // namespace nd::src::graphics::glfw
