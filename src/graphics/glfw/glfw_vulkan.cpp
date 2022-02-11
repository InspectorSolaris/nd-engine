#include "glfw_vulkan.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::glfw
{
    vec<str>
    getGlfwRequiredExtensions() noexcept
    {
        ND_SET_SCOPE();

        u32 count;

        const auto cextensions = glfwGetRequiredInstanceExtensions(&count);

        auto extensions = vec<str> {};

        extensions.reserve(count);

        std::transform(cextensions,
                       cextensions + count,
                       std::back_inserter(extensions),
                       [](const auto cextension)
                       {
                           return str {cextension};
                       });

        return extensions;
    }

    VkSurfaceKHR
    createSurface(const GlfwWindow window, const VkInstance instance)
    {
        ND_SET_SCOPE();

        VkSurfaceKHR surface;

        ND_VK_ASSERT_EXEC(glfwCreateWindowSurface(instance, window, ND_VK_ALLOCATION_CALLBACKS, &surface));

        return surface;
    }
} // namespace nd::src::graphics::glfw
