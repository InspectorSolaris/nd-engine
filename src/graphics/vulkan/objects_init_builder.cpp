#include "objects_init_builder.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VulkanObjectsInitBuilder::Type
    VulkanObjectsInitBuilder::get() const noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        ND_ASSERT(instance && physicalDevice && device && surface && swapchain && renderPass && swapchainImages && swapchainImageViews &&
                  swapchainFramebuffers && shaderModules && descriptorPool && descriptorSetLayout && pipelineCache && pipelineLayout && pipeline &&
                  commandPool);

        return {.instance              = instance,
                .physicalDevice        = physicalDevice,
                .device                = device,
                .surface               = surface,
                .swapchain             = swapchain,
                .renderPass            = renderPass,
                .swapchainImages       = swapchainImages,
                .swapchainImageViews   = swapchainImageViews,
                .swapchainFramebuffers = swapchainFramebuffers,
                .shaderModules         = shaderModules,
                .descriptorPool        = descriptorPool,
                .descriptorSetLayout   = descriptorSetLayout,
                .pipelineCache         = pipelineCache,
                .pipelineLayout        = pipelineLayout,
                .pipeline              = pipeline,
                .commandPool           = commandPool};
    }
} // namespace nd::src::graphics::vulkan
