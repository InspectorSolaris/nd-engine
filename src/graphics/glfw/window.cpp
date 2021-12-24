#include "window.hpp"
#include "tools.hpp"

namespace nd::src::graphics::glfw
{
    GlfwWindow
    getWindow(const WindowConfiguration& configuration) noexcept
    {
        ND_SET_SCOPE();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        return glfwCreateWindow(configuration.width, configuration.height, configuration.title.c_str(), nullptr, nullptr);
    }
} // namespace nd::src::graphics::glfw
