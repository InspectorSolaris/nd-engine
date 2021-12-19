#include "context.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    Context::Context(const Configuration& configuration)
        : instance_(std::move(configuration.instance))
        , device_(std::move(configuration.device))
        , surface_(std::move(configuration.surface))
        , swapchain_(std::move(configuration.swapchain))
        , renderPass_(std::move(configuration.renderPass))
        , swapchainImages_(std::move(configuration.swapchainImages))
        , swapchainImageViews_(std::move(configuration.swapchainImageViews))
        , swapchainFramebuffers_(std::move(configuration.swapchainFramebuffers))
        , shaderModules_(std::move(configuration.shaderModules))
        , descriptorPool_(std::move(configuration.descriptorPool))
        , descriptorSetLayout_(std::move(configuration.descriptorSetLayout))
        , descriptorSets_(std::move(configuration.descriptorSets))
        , pipelineLayout_(std::move(configuration.pipelineLayout))
        , pipelines_(std::move(configuration.pipelines))
    {
        ND_SET_SCOPE();
    }

    Context::~Context()
    {
        ND_SET_SCOPE();

        for(const auto pipeline: pipelines_)
        {
            vkDestroyPipeline(device_, pipeline, nullptr);
        }

        vkDestroyPipelineLayout(device_, pipelineLayout_, nullptr);

        vkFreeDescriptorSets(device_, descriptorPool_, descriptorSets_.size(), descriptorSets_.data());
        vkDestroyDescriptorSetLayout(device_, descriptorSetLayout_, nullptr);
        vkDestroyDescriptorPool(device_, descriptorPool_, nullptr);

        for(const auto shaderModule: shaderModules_)
        {
            vkDestroyShaderModule(device_, shaderModule, nullptr);
        }

        for(const auto swapchainFramebuffer: swapchainFramebuffers_)
        {
            vkDestroyFramebuffer(device_, swapchainFramebuffer, nullptr);
        }

        for(const auto swapchainImageView: swapchainImageViews_)
        {
            vkDestroyImageView(device_, swapchainImageView, nullptr);
        }

        vkDestroyRenderPass(device_, renderPass_, nullptr);
        vkDestroySwapchainKHR(device_, swapchain_, nullptr);
        vkDestroySurfaceKHR(instance_, surface_, nullptr);
        vkDestroyDevice(device_, nullptr);
        vkDestroyInstance(instance_, nullptr);
    }

    Context
    getContext(const ContextConfiguration& configuration)
    {
        ND_SET_SCOPE();

        const auto layers = getMerged(configuration.layers,
                                      {
#ifndef NDEBUG
                                          "VK_LAYER_KHRONOS_validation"
#endif
                                      });

        const auto extensions = getMerged(configuration.extensions, {});

        const auto instance = getInstance(InstanceConfiguration {configuration.applicationName,
                                                                 configuration.engineName,
                                                                 layers,
                                                                 extensions,
                                                                 VK_MAKE_VERSION(0, 1, 0),
                                                                 VK_MAKE_VERSION(0, 1, 0),
                                                                 VK_API_VERSION_1_2});

        const auto physicalDevicePriority = [](const VkPhysicalDevice            physicalDevice,
                                               const VkPhysicalDeviceProperties& properties,
                                               const VkPhysicalDeviceFeatures&   features)
        {
            return 1;
        };

        const auto [deviceQueueFamilies, physicalDevice, device] =
            getDevice({{}, physicalDevicePriority, {"VK_KHR_swapchain"}, VK_QUEUE_GRAPHICS_BIT}, instance);

        const auto surface             = configuration.getSurface(instance);
        const auto surfaceFormats      = getSurfaceFormats(physicalDevice, surface);
        const auto surfacePresentModes = getSurfacePresentModes(physicalDevice, surface);
        const auto surfaceCapabilities = getSurfaceCapabilities(physicalDevice, surface);

        const auto swapchainConfiguration = SwapchainConfiguration {deviceQueueFamilies,
                                                                    surfaceFormats,
                                                                    surfacePresentModes,
                                                                    surfaceCapabilities,
                                                                    physicalDevice,
                                                                    surface,
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

        const auto swapchain = getSwapchain(swapchainConfiguration, device);

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

        const auto renderPass = getRenderPass({renderPassAttachments, renderPassSubpasses, renderPassDependencies}, device);

        const auto swapchainImages = getSwapchainImages(device, swapchain);
        const auto swapchainImageViews =
            getMapped<VkImage, VkImageView>(swapchainImages,
                                            [device = device, &swapchainConfiguration](const auto image, const auto index)
                                            {
                                                return getImageView({{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
                                                                     {VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                      VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                      VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                      VK_COMPONENT_SWIZZLE_IDENTITY},
                                                                     VK_IMAGE_VIEW_TYPE_2D,
                                                                     swapchainConfiguration.imageFormat,
                                                                     image},
                                                                    device);
                                            });

        const auto swapchainFramebuffers = getMapped<VkImageView, VkFramebuffer>(
            swapchainImageViews,
            [device = device, renderPass, &swapchainConfiguration](const auto imageView, const auto index)
            {
                return getFramebuffer({{imageView},
                                       renderPass,
                                       swapchainConfiguration.imageExtent.width,
                                       swapchainConfiguration.imageExtent.height,
                                       swapchainConfiguration.imageArrayLayers},
                                      device);
            });

        const auto shaderPaths =
            std::vector<std::string> {"src/graphics/vulkan/shaders/vert.spv", "src/graphics/vulkan/shaders/frag.spv"};

        const auto shaderStages =
            std::vector<VkShaderStageFlagBits> {VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT};

        const auto shaderModules =
            getMapped<std::string, VkShaderModule>(shaderPaths,
                                                   [device = device](const auto& path, const auto index)
                                                   {
                                                       return getShaderModule({path}, device);
                                                   });

        const auto descriptorPool = getDescriptorPool(
            {{{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1}}, 1, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT},
            device);

        const auto descriptorSetLayout = getDescriptorSetLayout(DescriptorSetLayoutConfiguration {{}}, device);
        const auto descriptorSets      = getDescriptorSet({{descriptorSetLayout}, descriptorPool}, device);

        const auto pipelineLayout = getPipelineLayout({{descriptorSetLayout}, {}}, device);

        const auto vertexInputStateCreateInfo = getPipelineVertexInputStateCreateInfo(0, 0, nullptr, nullptr);
        const auto inputAssemblyStateCreateInfo =
            getPipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

        const auto tessellationStateCreateInfo = VkPipelineTessellationStateCreateInfo {};

        const auto scissors =
            VkRect2D {{0, 0}, {swapchainConfiguration.imageExtent.width, swapchainConfiguration.imageExtent.height}};
        const auto viewport = VkViewport {0.0f,
                                          0.0f,
                                          static_cast<float>(swapchainConfiguration.imageExtent.width),
                                          static_cast<float>(swapchainConfiguration.imageExtent.height),
                                          0.0f,
                                          0.1f};

        const auto viewportStateCreateInfo = getPipelineViewportStateCreateInfo(1, 1, &viewport, &scissors);

        const auto rasterizationStateCreateInfo = getPipelineRasterizationStateCreateInfo(VK_FALSE,
                                                                                          VK_FALSE,
                                                                                          VK_POLYGON_MODE_FILL,
                                                                                          VK_CULL_MODE_NONE,
                                                                                          VK_FRONT_FACE_CLOCKWISE,
                                                                                          VK_FALSE,
                                                                                          0.0f,
                                                                                          0.0f,
                                                                                          0.0f,
                                                                                          1.0f);

        const auto multisampleStateCreateInfo =
            getPipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f, nullptr, VK_FALSE, VK_FALSE);

        const auto depthStencilStateCreateInfo = VkPipelineDepthStencilStateCreateInfo {};

        const auto blendConstants       = std::vector<float> {0.0f, 0.0f, 0.0f, 0.0f};
        const auto colorBlendAttachment = VkPipelineColorBlendAttachmentState {
            VK_TRUE,
            VK_BLEND_FACTOR_SRC_ALPHA,
            VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            VK_BLEND_OP_ADD,
            VK_BLEND_FACTOR_ONE,
            VK_BLEND_FACTOR_ZERO,
            VK_BLEND_OP_ADD,
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};

        const auto colorBlendStateCreateInfo =
            getPipelineColorBlendStateCreateInfo(VK_FALSE, VK_LOGIC_OP_OR, 1, &colorBlendAttachment, blendConstants.data());

        const auto dynamicStateCreateInfo = getPipelineDynamicStateCreateInfo(0, nullptr);

        const auto pipelines = getGraphicsPipeline(
            {{getMapped<VkShaderModule, VkPipelineShaderStageCreateInfo>(
                  shaderModules,
                  [&shaderStages](const auto& shaderModule, const auto index)
                  {
                      return getPipelineShaderStageCreateInfo(shaderStages[index], shaderModule, "main", nullptr);
                  }),
              &vertexInputStateCreateInfo,
              &inputAssemblyStateCreateInfo,
              nullptr,
              &viewportStateCreateInfo,
              &rasterizationStateCreateInfo,
              &multisampleStateCreateInfo,
              nullptr,
              &colorBlendStateCreateInfo,
              &dynamicStateCreateInfo,
              pipelineLayout,
              renderPass,
              0}},
            device);

        return Context({std::move(swapchainImages),
                        std::move(swapchainImageViews),
                        std::move(swapchainFramebuffers),
                        std::move(shaderModules),
                        std::move(descriptorSets),
                        std::move(pipelines),
                        instance,
                        device,
                        surface,
                        swapchain,
                        renderPass,
                        descriptorPool,
                        descriptorSetLayout,
                        pipelineLayout});
    }
} // namespace nd::src::graphics::vulkan
