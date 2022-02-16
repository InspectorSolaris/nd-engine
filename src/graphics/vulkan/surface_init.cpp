#include "surface_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    Surface
    createSurface(const VkInstance instance) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        ND_ASSERT_STATIC();

        return VK_NULL_HANDLE;
    }
} // namespace nd::src::graphics::vulkan
