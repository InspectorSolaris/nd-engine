#include "context.hpp"
#include "tools.hpp"

namespace nd::src::graphics::glfw
{
    bool GlfwContext::s_initialized {};

    GlfwContext::GlfwContext()
    {
        ND_SET_SCOPE();

        ND_ASSERT(!s_initialized);

        glfwInit();

        s_initialized = true;
    }

    GlfwContext::~GlfwContext()
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
    getSurface(const GlfwWindow window, const VkInstance instance)
    {
        ND_SET_SCOPE();

        VkSurfaceKHR surface;

        ND_ASSERT(glfwCreateWindowSurface(instance, window, nullptr, &surface) == VK_SUCCESS);

        return surface;
    }

    GlfwContext
    getGlfwContext() noexcept
    {
        ND_SET_SCOPE();

        return GlfwContext();
    }
} // namespace nd::src::graphics::glfw
