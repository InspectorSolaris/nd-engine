#include "objects_cfg_builder.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    ObjectsCfgBuilder::Type
    ObjectsCfgBuilder::get() const noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        ND_ASSERT(instance && physicalDevice && device && buffer && swapchain && renderPass && swapchainImageView && swapchainFramebuffer &&
                  descriptorPool && descriptorSetLayout && shaderModules && pipelineCache && pipelineLayout && pipeline && commandPool);

        return {.instance             = instance,
                .physicalDevice       = physicalDevice,
                .device               = device,
                .buffer               = buffer,
                .swapchain            = swapchain,
                .renderPass           = renderPass,
                .swapchainImageView   = swapchainImageView,
                .swapchainFramebuffer = swapchainFramebuffer,
                .descriptorPool       = descriptorPool,
                .descriptorSetLayout  = descriptorSetLayout,
                .shaderModules        = shaderModules,
                .pipelineCache        = pipelineCache,
                .pipelineLayout       = pipelineLayout,
                .pipeline             = pipeline,
                .commandPool          = commandPool};
    }
} // namespace nd::src::graphics::vulkan
