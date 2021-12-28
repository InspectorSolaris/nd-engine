#pragma once

#include "pch.hpp"
#include "window.hpp"

namespace nd::src::graphics::glfw
{
    class GlfwContext
    {
    public:
        GlfwContext();

        GlfwContext(const GlfwContext& context) = delete;
        GlfwContext(GlfwContext&& context)      = delete;

        GlfwContext&
        operator=(const GlfwContext& context) = delete;
        GlfwContext&
        operator=(GlfwContext&& context) = delete;

        ~GlfwContext();

    private:
        static bool s_initialized;
    };

    std::vector<std::string>
    getRequiredExtensions() noexcept;

    VkSurfaceKHR
    getSurface(const GlfwWindow window, const VkInstance instance);

    GlfwContext
    getGlfwContext() noexcept;
} // namespace nd::src::graphics::glfw
