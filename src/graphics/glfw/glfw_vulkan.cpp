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
    createSurface(const GlfwWindow window, const VkInstance instance, const VkAllocationCallbacks* allocationCallbacks)
    {
        ND_SET_SCOPE();

        VkSurfaceKHR surface;

        ND_ASSERT_EXEC(glfwCreateWindowSurface(instance, window, allocationCallbacks, &surface) == VK_SUCCESS);

        return surface;
    }
} // namespace nd::src::graphics::glfw
