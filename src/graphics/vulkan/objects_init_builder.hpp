#pragma once

#include "pch.hpp"
#include "tools.hpp"

#include "objects_init.hpp"

namespace nd::src::graphics::vulkan
{
    class VulkanObjectsInitBuilder final
    {
    public:
        using Type    = VulkanObjectsInit;
        using Builder = VulkanObjectsInitBuilder;

        static Builder
        getDefault() noexcept(ND_ASSERT_NOTHROW)
        {
            return Builder {} << createInstance << getPhysicalDevice << createDevice << createSwapchain << createRenderPass << getSwapchainImages
                              << createSwapchainImageViews << createSwapchainFramebuffers << createShaderModules << createPipelineCache;
        }

        operator Type() const noexcept(ND_ASSERT_NOTHROW)
        {
            return get();
        }

        Type
        get() const noexcept(ND_ASSERT_NOTHROW);

        ND_DEFINE_BUILDER_SET(instance);
        ND_DEFINE_BUILDER_SET(physicalDevice);
        ND_DEFINE_BUILDER_SET(device);
        ND_DEFINE_BUILDER_SET(surface);
        ND_DEFINE_BUILDER_SET(swapchain);
        ND_DEFINE_BUILDER_SET(renderPass);
        ND_DEFINE_BUILDER_SET(swapchainImages);
        ND_DEFINE_BUILDER_SET(swapchainImageViews);
        ND_DEFINE_BUILDER_SET(swapchainFramebuffers);
        ND_DEFINE_BUILDER_SET(shaderModules);
        ND_DEFINE_BUILDER_SET(pipelineCache);

        ND_DEFINE_BUILDER_OPERATOR(instance);
        ND_DEFINE_BUILDER_OPERATOR(physicalDevice);
        ND_DEFINE_BUILDER_OPERATOR(device);
        ND_DEFINE_BUILDER_OPERATOR(surface);
        ND_DEFINE_BUILDER_OPERATOR(swapchain);
        ND_DEFINE_BUILDER_OPERATOR(renderPass);
        ND_DEFINE_BUILDER_OPERATOR(swapchainImages);
        ND_DEFINE_BUILDER_OPERATOR(swapchainImageViews);
        ND_DEFINE_BUILDER_OPERATOR(swapchainFramebuffers);
        ND_DEFINE_BUILDER_OPERATOR(shaderModules);
        ND_DEFINE_BUILDER_OPERATOR(pipelineCache);

    private:
        ND_DECLARE_BUILDER_FIELD(instance);
        ND_DECLARE_BUILDER_FIELD(physicalDevice);
        ND_DECLARE_BUILDER_FIELD(device);
        ND_DECLARE_BUILDER_FIELD(surface);
        ND_DECLARE_BUILDER_FIELD(swapchain);
        ND_DECLARE_BUILDER_FIELD(renderPass);
        ND_DECLARE_BUILDER_FIELD(swapchainImages);
        ND_DECLARE_BUILDER_FIELD(swapchainImageViews);
        ND_DECLARE_BUILDER_FIELD(swapchainFramebuffers);
        ND_DECLARE_BUILDER_FIELD(shaderModules);
        ND_DECLARE_BUILDER_FIELD(pipelineCache);
    };
} // namespace nd::src::graphics::vulkan
