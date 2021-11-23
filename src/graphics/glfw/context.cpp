#include "context.hpp"
#include "tools.hpp"

namespace nd::src::graphics::glfw
{
    Context::Context(Window&& window) noexcept
        : window_(std::move(window))
    {
        ND_SET_SCOPE_LOW();
    }

    Context::~Context()
    {
        ND_SET_SCOPE_LOW();
    }

    Context::Extensions
    getRequiredExtensions() noexcept
    {
        ND_SET_SCOPE_LOW();

        uint32_t count;

        const auto cextensions = glfwGetRequiredInstanceExtensions(&count);

        auto extensions = Context::Extensions {};

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
        ND_SET_SCOPE_LOW();

        VkSurfaceKHR surface;

        ND_ASSERT(glfwCreateWindowSurface(instance, window.get(), nullptr, &surface) == VK_SUCCESS);

        return surface;
    }

    Context
    getContext(const Context::Configuration& configuration) noexcept
    {
        auto window = glfw::getWindow({configuration.title, configuration.width, configuration.height});

        return Context(std::move(window));
    }
} // namespace nd::src::graphics::glfw
