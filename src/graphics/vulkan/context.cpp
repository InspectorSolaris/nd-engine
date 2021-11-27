#include "context.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    Context::Context(Instance&&                instance,
                     Device&&                  device,
                     Surface&&                 surface,
                     RenderPass&&              renderPass,
                     Swapchain&&               swapchain,
                     Swapchain::Images&&       swapchainImages,
                     Swapchain::ImageViews&&   swapchainImageViews,
                     Swapchain::Framebuffers&& swapchainFramebuffers,
                     ShaderModules&&           shaderModules)
        : instance_(std::move(instance))
        , device_(std::move(device))
        , surface_(std::move(surface))
        , renderPass_(std::move(renderPass))
        , swapchain_(std::move(swapchain))
        , swapchainImages_(std::move(swapchainImages))
        , swapchainFramebuffers_(std::move(swapchainFramebuffers))
        , shaderModules_(std::move(shaderModules))
    {
        ND_SET_SCOPE_LOW();
    }

    Context::~Context()
    {
        ND_SET_SCOPE_LOW();
    }

    Context
    getContext(const Context::Configuration& configuration)
    {
        ND_SET_SCOPE_LOW();

        const auto instanceLayers = Layers {
#ifndef NDEBUG
            "VK_LAYER_KHRONOS_validation"
#endif
        };
        const auto instanceExtensions = Extensions {};

        auto instance = getInstance({configuration.applicationName,
                                     configuration.engineName,
                                     getMerged(instanceLayers, configuration.layers),
                                     getMerged(instanceExtensions, configuration.extensions),
                                     VK_MAKE_VERSION(0, 1, 0),
                                     VK_MAKE_VERSION(0, 1, 0),
                                     VK_API_VERSION_1_2});

        const auto physicalDevicePriority = [](const auto& physicalDevice)
        {
            return 1;
        };

        auto device = getDevice({{}, physicalDevicePriority, {"VK_KHR_swapchain"}, VK_QUEUE_GRAPHICS_BIT}, instance.get());

        auto surface = getSurface(instance.get(), configuration.getSurface(instance.get()));

        const auto swapchainConfiguration = Swapchain::Configuration {{configuration.width, configuration.height},
                                                                      1,
                                                                      1,
                                                                      true,
                                                                      VK_FORMAT_B8G8R8A8_SRGB,
                                                                      VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
                                                                      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                                                      VK_PRESENT_MODE_IMMEDIATE_KHR,
                                                                      VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                                                                      VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR};

        auto swapchain = getSwapchain(swapchainConfiguration,
                                      device.getQueueFamilies(),
                                      device.getPhysical(),
                                      device.get(),
                                      surface.get());

        auto swapchainImages     = getSwapchainImages(device.get(), swapchain.get());
        auto swapchainImageViews = getSwapchainImageViews(device.get(), swapchainImages, swapchainConfiguration);

        auto colorAttachments = RenderPass::AttachmentReferences {{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}};

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

        auto renderPass = getRenderPass({renderPassAttachments, renderPassSubpasses, renderPassDependencies}, device.get());

        auto swapchainFramebuffers =
            getSwapchainFramebuffers(device.get(), renderPass.get(), swapchainImageViews, swapchainConfiguration);

        const auto shadersEntryPoint = "main";
        const auto shadersDir        = "src/graphics/vulkan/shaders";

        const auto shaderModuleConfigurations = std::vector<ShaderModule::Configuration> {
            {fmt::format("{}/vert.spv", shadersDir), VK_SHADER_STAGE_VERTEX_BIT},
            {fmt::format("{}/frag.spv", shadersDir), VK_SHADER_STAGE_FRAGMENT_BIT}};

        auto shaderModules = Context::ShaderModules {};

        shaderModules.reserve(shaderModuleConfigurations.size());

        for(const auto& shaderModuleConfiguration: shaderModuleConfigurations)
        {
            shaderModules.push_back(getShaderModule(shaderModuleConfiguration, device.get()));
        }

        return Context(std::move(instance),
                       std::move(device),
                       std::move(surface),
                       std::move(renderPass),
                       std::move(swapchain),
                       std::move(swapchainImages),
                       std::move(swapchainImageViews),
                       std::move(swapchainFramebuffers),
                       std::move(shaderModules));
    }
} // namespace nd::src::graphics::vulkan
