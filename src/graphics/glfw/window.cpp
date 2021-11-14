#include "window.hpp"
#include "tools.hpp"

namespace nd::src::graphics::glfw
{
    Window::Window() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    Window::Window(int width, int height, const char* title) noexcept
    {
        ND_SET_SCOPE_LOW();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    }

    Window::Window(Window&& window) noexcept
        : window_(std::move(window.window_))
        , width_(std::move(window.width_))
        , height_(std::move(window.height_))
    {
        ND_SET_SCOPE_LOW();

        window.window_ = nullptr;
        window.width_  = 0;
        window.height_ = 0;
    }

    Window&
    Window::operator=(Window&& window) noexcept
    {
        ND_SET_SCOPE_LOW();

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
        ND_SET_SCOPE_LOW();

        if(window_ != nullptr)
        {
            glfwDestroyWindow(window_);
        }
    }

    VkSurfaceKHR
    Window::getSurface(const VkInstance instance) const
    {
        ND_SET_SCOPE_LOW();

        VkSurfaceKHR surface;

        if(glfwCreateWindowSurface(instance, window_, nullptr, &surface) != VK_SUCCESS)
        {
            // TODO: Refactor exceptional cases
            throw 0;
        }

        return surface;
    }
} // namespace nd::src::graphics::glfw
