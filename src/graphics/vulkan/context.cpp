#include "context.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    Context::Context(Instance&&                  instance,
                     Device&&                    device,
                     Surface&&                   surface,
                     Swapchain&&                 swapchain,
                     RenderPass&&                renderPass,
                     std::vector<VkImage>&&      swapchainImages,
                     std::vector<ImageView>&&    swapchainImageViews,
                     std::vector<Framebuffer>&&  swapchainFramebuffers,
                     std::vector<ShaderModule>&& shaderModules,
                     DescriptorPool&&            descriptorPool,
                     DescriptorSetLayout&&       descriptorSetLayout,
                     DescriptorSet&&             descriptorSet,
                     PipelineLayout&&            pipelineLayout,
                     Pipeline&&                  pipeline)
        : instance_(std::move(instance))
        , device_(std::move(device))
        , surface_(std::move(surface))
        , swapchain_(std::move(swapchain))
        , renderPass_(std::move(renderPass))
        , swapchainImages_(std::move(swapchainImages))
        , swapchainFramebuffers_(std::move(swapchainFramebuffers))
        , shaderModules_(std::move(shaderModules))
        , descriptorPool_(std::move(descriptorPool))
        , descriptorSetLayout_(std::move(descriptorSetLayout))
        , descriptorSet_(std::move(descriptorSet))
        , pipelineLayout_(std::move(pipelineLayout))
        , pipeline_(std::move(pipeline))
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

        const auto layers = std::vector<std::string> {
#ifndef NDEBUG
            "VK_LAYER_KHRONOS_validation"
#endif
        };
        const auto extensions = std::vector<std::string> {};

        auto instance = getInstance({configuration.applicationName,
                                     configuration.engineName,
                                     getMerged(layers, configuration.layers),
                                     getMerged(extensions, configuration.extensions),
                                     VK_MAKE_VERSION(0, 1, 0),
                                     VK_MAKE_VERSION(0, 1, 0),
                                     VK_API_VERSION_1_2});

        const auto physicalDevicePriority = [](const auto& physicalDevice)
        {
            return 1;
        };

        auto device = getDevice({{}, physicalDevicePriority, {"VK_KHR_swapchain"}, VK_QUEUE_GRAPHICS_BIT}, instance.get());

        auto surface = getSurface(instance.get(), configuration.getSurface(instance.get()));

        const auto surfaceFormats      = getSurfaceFormats(device.getPhysical(), surface.get());
        const auto surfacePresentModes = getSurfacePresentModes(device.getPhysical(), surface.get());
        const auto surfaceCapabilities = getSurfaceCapabilities(device.getPhysical(), surface.get());

        const auto swapchainConfiguration = Swapchain::Configuration {surfaceFormats,
                                                                      surfacePresentModes,
                                                                      surfaceCapabilities,
                                                                      device.getPhysical(),
                                                                      surface.get(),
                                                                      {configuration.width, configuration.height},
                                                                      1,
                                                                      1,
                                                                      true,
                                                                      VK_FORMAT_B8G8R8A8_SRGB,
                                                                      VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
                                                                      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                                                      VK_PRESENT_MODE_IMMEDIATE_KHR,
                                                                      VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                                                                      VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR};

        auto swapchain = getSwapchain(swapchainConfiguration, device.getQueueFamilies(), device.get());

        const auto colorAttachments = std::vector<VkAttachmentReference> {{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}};

        const auto renderPassAttachments =
            std::vector<VkAttachmentDescription> {getRenderPassAttachment(swapchainConfiguration.imageFormat,
                                                                          VK_SAMPLE_COUNT_1_BIT,
                                                                          VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                                          VK_ATTACHMENT_STORE_OP_STORE,
                                                                          VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                                          VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                                          VK_IMAGE_LAYOUT_UNDEFINED,
                                                                          VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)};

        const auto renderPassSubpasses =
            std::vector<VkSubpassDescription> {getRenderPassSubpass(VK_PIPELINE_BIND_POINT_GRAPHICS,
                                                                    0,
                                                                    colorAttachments.size(),
                                                                    0,
                                                                    nullptr,
                                                                    colorAttachments.data(),
                                                                    nullptr,
                                                                    nullptr,
                                                                    nullptr)};

        const auto renderPassDependencies = std::vector<VkSubpassDependency> {};

        auto renderPass = getRenderPass({renderPassAttachments, renderPassSubpasses, renderPassDependencies}, device.get());

        auto swapchainImages = getSwapchainImages(device.get(), swapchain.get());

        auto swapchainImageViews =
            getMapped<VkImage, ImageView>(swapchainImages,
                                          [&device, &swapchainConfiguration](const auto image)
                                          {
                                              return getImageView({{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
                                                                   {VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                    VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                    VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                    VK_COMPONENT_SWIZZLE_IDENTITY},
                                                                   VK_IMAGE_VIEW_TYPE_2D,
                                                                   swapchainConfiguration.imageFormat,
                                                                   image},
                                                                  device.get());
                                          });

        auto swapchainFramebuffers =
            getMapped<ImageView, Framebuffer>(swapchainImageViews,
                                              [&device, &renderPass, &swapchainConfiguration](const auto& imageView)
                                              {
                                                  return getFramebuffer({{imageView.get()},
                                                                         renderPass.get(),
                                                                         swapchainConfiguration.imageExtent.width,
                                                                         swapchainConfiguration.imageExtent.height,
                                                                         1},
                                                                        device.get());
                                              });

        const auto shadersEntryPoint = "main";
        const auto shadersDir        = "src/graphics/vulkan/shaders";

        const auto shaderModuleConfigurations = std::vector<ShaderModule::Configuration> {
            {fmt::format("{}/vert.spv", shadersDir), VK_SHADER_STAGE_VERTEX_BIT},
            {fmt::format("{}/frag.spv", shadersDir), VK_SHADER_STAGE_FRAGMENT_BIT}};

        auto shaderModules = getMapped<ShaderModule::Configuration, ShaderModule>(
            shaderModuleConfigurations,
            [&device](const auto& shaderModuleConfiguration)
            {
                return getShaderModule(shaderModuleConfiguration, device.get());
            });

        auto descriptorPool      = getDescriptorPool({{{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1}}, 1}, device.get());
        auto descriptorSetLayout = getDescriptorSetLayout({{}}, device.get());
        auto descriptorSet       = getDescriptorSet({{}, descriptorPool.get()}, device.get());

        return Context(std::move(instance),
                       std::move(device),
                       std::move(surface),
                       std::move(swapchain),
                       std::move(renderPass),
                       std::move(swapchainImages),
                       std::move(swapchainImageViews),
                       std::move(swapchainFramebuffers),
                       std::move(shaderModules),
                       std::move(descriptorPool),
                       std::move(descriptorSetLayout),
                       std::move(descriptorSet),
                       PipelineLayout {},
                       Pipeline {});
    }
} // namespace nd::src::graphics::vulkan
