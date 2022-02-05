#include "objects_cfg_builder.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VulkanObjectsCfgBuilder::Type
    VulkanObjectsCfgBuilder::get() const noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        ND_ASSERT(instance && physicalDevice && device && swapchain && renderPass && swapchainImageView && swapchainFramebuffer && shaderModules &&
                  descriptorPool && descriptorSetLayout && pipelineCache && pipelineLayout && pipeline);

        return {.instance             = instance,
                .physicalDevice       = physicalDevice,
                .device               = device,
                .swapchain            = swapchain,
                .renderPass           = renderPass,
                .swapchainImageView   = swapchainImageView,
                .swapchainFramebuffer = swapchainFramebuffer,
                .shaderModules        = shaderModules,
                .descriptorPool       = descriptorPool,
                .descriptorSetLayout  = descriptorSetLayout,
                .pipelineCache        = pipelineCache,
                .pipelineLayout       = pipelineLayout,
                .pipeline             = pipeline};
    }
} // namespace nd::src::graphics::vulkan
