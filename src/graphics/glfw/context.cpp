#include "context.hpp"
#include "tools.hpp"

namespace nd::src::graphics::glfw
{
    std::vector<std::string>
    getRequiredExtensions() noexcept
    {
        ND_SET_SCOPE();

        uint32_t count;

        const auto cextensions = glfwGetRequiredInstanceExtensions(&count);

        auto extensions = std::vector<std::string> {};

        extensions.reserve(count);

        std::transform(cextensions,
                       cextensions + count,
                       std::back_inserter(extensions),
                       [](const auto cextension)
                       {
                           return std::string(cextension);
                       });

        return extensions;
    }

    VkSurfaceKHR
    getSurface(const GlfwWindow window, const VkInstance instance)
    {
        ND_SET_SCOPE();

        VkSurfaceKHR surface;

        ND_ASSERT(glfwCreateWindowSurface(instance, window, nullptr, &surface) == VK_SUCCESS);

        return surface;
    }
} // namespace nd::src::graphics::glfw
