#include "context.hpp"
#include "tools.hpp"

namespace nd::src::graphics::glfw
{
    Context::Context() noexcept
        : window_()
    {
        ND_SET_SCOPE_LOW();

        glfwInit();

        window_ = Window(800, 600, "nd-engine");
    }

    Context::~Context()
    {
        ND_SET_SCOPE_LOW();

        glfwTerminate();
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
} // namespace nd::src::graphics::glfw
