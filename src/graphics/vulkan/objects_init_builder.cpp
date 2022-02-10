#include "objects_init_builder.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VulkanObjectsInitBuilder::Type
    VulkanObjectsInitBuilder::get() const noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        ND_ASSERT(instance && physicalDevice && device && buffer && surface && swapchain && renderPass && swapchainImages && swapchainImageViews &&
                  swapchainFramebuffers && descriptorPool && descriptorSetLayout && shaderModules && pipelineCache && pipelineLayout && pipeline &&
                  commandPool);

        return {.instance              = instance,
                .physicalDevice        = physicalDevice,
                .device                = device,
                .buffer                = buffer,
                .surface               = surface,
                .swapchain             = swapchain,
                .renderPass            = renderPass,
                .swapchainImages       = swapchainImages,
                .swapchainImageViews   = swapchainImageViews,
                .swapchainFramebuffers = swapchainFramebuffers,
                .descriptorPool        = descriptorPool,
                .descriptorSetLayout   = descriptorSetLayout,
                .shaderModules         = shaderModules,
                .pipelineCache         = pipelineCache,
                .pipelineLayout        = pipelineLayout,
                .pipeline              = pipeline,
                .commandPool           = commandPool};
    }
} // namespace nd::src::graphics::vulkan
