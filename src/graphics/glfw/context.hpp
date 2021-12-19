#pragma once

#include "pch.hpp"
#include "window.hpp"

namespace nd::src::graphics::glfw
{
    class Context
    {
    public:
        Context();

        Context(const Context& context) = delete;
        Context(Context&& context)      = delete;

        Context&
        operator=(const Context& context) = delete;
        Context&
        operator=(Context&& context) = delete;

        ~Context();

    private:
        static bool s_initialized;
    };

    std::vector<std::string>
    getRequiredExtensions() noexcept;

    VkSurfaceKHR
    getSurface(const Window& window, const VkInstance instance);

    Context
    getContext() noexcept;
} // namespace nd::src::graphics::glfw
