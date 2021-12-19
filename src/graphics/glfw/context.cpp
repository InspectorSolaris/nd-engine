#include "context.hpp"
#include "tools.hpp"

namespace nd::src::graphics::glfw
{
    bool Context::s_initialized {};

    Context::Context()
    {
        ND_SET_SCOPE();

        ND_ASSERT(!s_initialized);

        glfwInit();

        s_initialized = true;
    }

    Context::~Context()
    {
        ND_SET_SCOPE();

        glfwTerminate();

        s_initialized = false;
    }

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
    getSurface(const Window& window, const VkInstance instance)
    {
        ND_SET_SCOPE();

        VkSurfaceKHR surface;

        ND_ASSERT(glfwCreateWindowSurface(instance, window.get(), nullptr, &surface) == VK_SUCCESS);

        return surface;
    }

    Context
    getContext() noexcept
    {
        return Context();
    }
} // namespace nd::src::graphics::glfw
