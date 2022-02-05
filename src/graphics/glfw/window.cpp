#include "window.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::glfw
{
    Window
    getWindow(const WindowCfg& cfg) noexcept
    {
        ND_SET_SCOPE();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        return {.handle = glfwCreateWindow(cfg.width, cfg.height, cfg.title.c_str(), nullptr, nullptr), .width = cfg.width, .height = cfg.height};
    }
} // namespace nd::src::graphics::glfw
