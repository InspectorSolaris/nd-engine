#include "context.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    Context::Context(const Context::Configuration& configuration)
    {
        ND_SET_SCOPE_LOW();

        const auto instanceLayers = std::vector<std::string> {
#ifndef NDEBUG
            "VK_LAYER_KHRONOS_validation"
#endif
        };
        const auto instanceExtensions = std::vector<std::string> {};

        instance_ = getInstance({"nd-engine",
                                 "nd-engine",
                                 getMerged(instanceLayers, configuration.layers),
                                 getMerged(instanceExtensions, configuration.extensions),
                                 VK_MAKE_VERSION(0, 1, 0),
                                 VK_MAKE_VERSION(0, 1, 0),
                                 VK_API_VERSION_1_2});

        const auto physicalDevicePriority = [](const auto& physicalDevice)
        {
            return 1;
        };

        device_ = getDevice({{}, physicalDevicePriority, {"VK_KHR_swapchain"}, VK_QUEUE_GRAPHICS_BIT}, instance_.get());

        surface_ = getSurface(instance_.get(), configuration.getSurface(instance_.get()));

        const auto swapchainConfiguration = Swapchain::Configuration {
            {configuration.width, configuration.height},
            1,
            1,
            true,
            VK_FORMAT_B8G8R8A8_UNORM,
            VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            VK_PRESENT_MODE_IMMEDIATE_KHR,
            VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
            VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
        };

        swapchain_ = getSwapchain(swapchainConfiguration,
                                  device_.getQueueFamilies(),
                                  device_.getPhysical(),
                                  device_.get(),
                                  surface_.get());

        swapchainImages_     = getSwapchainImages(device_.get(), swapchain_.get());
        swapchainImageViews_ = getSwapchainImageViews(device_.get(), swapchainImages_, swapchainConfiguration);

        auto colorAttachments =
            RenderPass::AttachmentReferences {getRenderPassAttachmentReference(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)};

        auto renderPassAttachments = RenderPass::Attachments {getRenderPassAttachment(swapchainConfiguration.imageFormat,
                                                                                      VK_SAMPLE_COUNT_1_BIT,
                                                                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                                                      VK_ATTACHMENT_STORE_OP_STORE,
                                                                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                                                                      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)};

        auto renderPassSubpasses = RenderPass::Subpasses {getRenderPassSubpass(VK_PIPELINE_BIND_POINT_GRAPHICS,
                                                                               0,
                                                                               1,
                                                                               0,
                                                                               nullptr,
                                                                               colorAttachments.data(),
                                                                               nullptr,
                                                                               nullptr,
                                                                               nullptr)};

        auto renderPassDependencies = RenderPass::Dependencies {};

        renderPass_ = getRenderPass({renderPassAttachments, renderPassSubpasses, renderPassDependencies}, device_.get());

        swapchainFramebuffers_ =
            getSwapchainFramebuffers(device_.get(), renderPass_.get(), swapchainImageViews_, swapchainConfiguration);
    }

    Context::~Context()
    {
    }
} // namespace nd::src::graphics::vulkan
