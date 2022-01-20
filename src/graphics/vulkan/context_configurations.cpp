#include "context_configurations.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    InstanceConfiguration
    getInstanceConfiguration(const VulkanConfigurationExternal& configurationExternal) noexcept
    {
        ND_SET_SCOPE();

        auto layers = getMerged(configurationExternal.layers,
                                {
#ifndef NDEBUG
                                    "VK_LAYER_KHRONOS_validation"
#endif
                                });

        auto extensions = getMerged(configurationExternal.extensions, {});

        return {configurationExternal.applicationName,
                configurationExternal.engineName,
                std::move(layers),
                std::move(extensions),
                VK_MAKE_VERSION(0, 1, 0),
                VK_MAKE_VERSION(0, 1, 0),
                VK_API_VERSION_1_2};
    }

    PhysicalDeviceConfiguration
    getPhysicalDeviceConfiguration() noexcept
    {
        ND_SET_SCOPE();

        return {{},
                [](const VkPhysicalDevice physicalDevice, const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceFeatures& features)
                {
                    return 1;
                },
                {"VK_KHR_swapchain"},
                VK_QUEUE_GRAPHICS_BIT};
    }

    DeviceConfiguration
    getDeviceConfiguration(const PhysicalDeviceConfiguration& physicalDeviceConfiguration) noexcept
    {
        ND_SET_SCOPE();

        return {physicalDeviceConfiguration.features, physicalDeviceConfiguration.extensions};
    }

    std::vector<DeviceMemoryConfiguration>
    getDeviceMemoryConfigurations(const VkPhysicalDeviceMemoryProperties* physicalDeviceMemoryProperties) noexcept
    {
        ND_SET_SCOPE();

        return getFiltered<DeviceMemoryConfiguration>(
            getMapped<DeviceMemoryConfiguration>(physicalDeviceMemoryProperties->memoryTypeCount,
                                                 [physicalDeviceMemoryProperties](const auto index)
                                                 {
                                                     return DeviceMemoryConfiguration {
                                                         index < physicalDeviceMemoryProperties->memoryTypeCount &&
                                                                 physicalDeviceMemoryProperties->memoryTypes[index].propertyFlags
                                                             ? static_cast<VkDeviceSize>(128 * 1024)
                                                             : static_cast<VkDeviceSize>(0),
                                                         static_cast<uint32_t>(index)};
                                                 }),
            [](const auto& configuration, const auto index)
            {
                return configuration.size;
            });
    }

    SwapchainConfiguration
    getSwapchainConfiguration(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface, const uint32_t width, const uint32_t height) noexcept
    {
        ND_SET_SCOPE();

        return {physicalDevice,
                surface,
                {width, height},
                1,
                1,
                true,
                VK_FORMAT_B8G8R8A8_SRGB,
                VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                VK_PRESENT_MODE_IMMEDIATE_KHR,
                VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR};
    }

    RenderPassConfiguration
    getRenderPassConfiguration(const SwapchainConfiguration& swapchainConfiguration) noexcept
    {
        ND_SET_SCOPE();

        using Attachments  = std::vector<AttachmentDescription>;
        using Subpasses    = std::vector<SubpassDescription>;
        using Dependencies = std::vector<SubpassDependency>;

        const auto attachments = Attachments {getRenderPassAttachment(swapchainConfiguration.imageFormat,
                                                                      VK_SAMPLE_COUNT_1_BIT,
                                                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                                      VK_ATTACHMENT_STORE_OP_STORE,
                                                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                                                      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)};

        const auto subpasses = Subpasses {{{}, {{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}}, {}, {}, {}, VK_PIPELINE_BIND_POINT_GRAPHICS}};

        const auto dependencies = Dependencies {{VK_SUBPASS_EXTERNAL,
                                                 0,
                                                 VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                                 VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                                 0,
                                                 VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                                                 {}}};

        return {attachments, subpasses, dependencies};
    }

    ImageViewConfiguration
    getSwapchainImageViewConfiguration(const SwapchainConfiguration& swapchainConfiguration, const VkImage image) noexcept
    {
        ND_SET_SCOPE();

        return {{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
                {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY},
                VK_IMAGE_VIEW_TYPE_2D,
                swapchainConfiguration.imageFormat,
                image};
    }

    std::vector<ImageViewConfiguration>
    getSwapchainImageViewConfigurations(const SwapchainConfiguration& swapchainConfiguration, const std::vector<Image>& images) noexcept
    {
        ND_SET_SCOPE();

        return getMapped<Image, ImageViewConfiguration>(images,
                                                        [&swapchainConfiguration](const auto image, const auto index)
                                                        {
                                                            return getSwapchainImageViewConfiguration(swapchainConfiguration, image);
                                                        });
    }

    FramebufferConfiguration
    getSwapchainFramebufferConfiguration(const SwapchainConfiguration& swapchainConfiguration,
                                         const VkImageView             imageView,
                                         const VkRenderPass            renderPass) noexcept
    {
        ND_SET_SCOPE();

        return {{imageView},
                renderPass,
                swapchainConfiguration.imageExtent.width,
                swapchainConfiguration.imageExtent.height,
                swapchainConfiguration.imageArrayLayers};
    }

    std::vector<FramebufferConfiguration>
    getSwapchainFramebufferConfigurations(const SwapchainConfiguration& swapchainConfiguration,
                                          const std::vector<ImageView>& imageViews,
                                          const VkRenderPass            renderPass) noexcept
    {
        ND_SET_SCOPE();

        return getMapped<ImageView, FramebufferConfiguration>(
            imageViews,
            [&swapchainConfiguration, renderPass](const auto imageView, const auto index)
            {
                return getSwapchainFramebufferConfiguration(swapchainConfiguration, imageView, renderPass);
            });
    }

    std::vector<ShaderModuleConfiguration>
    getShaderModulesConfigurations() noexcept
    {
        ND_SET_SCOPE();

        return {{"src/graphics/vulkan/shaders/vert.spv", VK_SHADER_STAGE_VERTEX_BIT},
                {"src/graphics/vulkan/shaders/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT}};
    }

    DescriptorPoolConfiguration
    getDescriptorPoolConfiguration() noexcept
    {
        ND_SET_SCOPE();

        return {{{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1}}, 1, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT};
    }

    std::vector<DescriptorSetLayoutConfiguration>
    getDescriptorSetLayoutConfigurations() noexcept
    {
        ND_SET_SCOPE();

        return {{}};
    }

    DescriptorSetConfiguration
    getDescriptorSetConfiguration(const std::vector<DescriptorSetLayout>& descriptorSetLayouts, const VkDescriptorPool descriptorPool) noexcept
    {
        ND_SET_SCOPE();

        return {getMapped<DescriptorSetLayout, VkDescriptorSetLayout>(descriptorSetLayouts,
                                                                      [](const auto descriptorSetLayout, const auto index)
                                                                      {
                                                                          return descriptorSetLayout;
                                                                      }),
                descriptorPool};
    }

    std::vector<PipelineLayoutConfiguration>
    getPipelineLayoutConfigurations(const std::vector<DescriptorSetLayout>& descriptorSetLayouts) noexcept
    {
        ND_SET_SCOPE();

        return {{getMapped<DescriptorSetLayout, VkDescriptorSetLayout>(descriptorSetLayouts,
                                                                       [](const auto descriptorSetLayout, const auto index)
                                                                       {
                                                                           return descriptorSetLayout;
                                                                       }),
                 {}}};
    }

    std::vector<GraphicsPipelineConfiguration>
    getGraphicsPipelineConfigurations(const std::vector<ShaderModule>&   shaderModules,
                                      const std::vector<PipelineLayout>& pipelineLayouts,
                                      const VkRenderPass                 renderPass,
                                      const uint32_t                     width,
                                      const uint32_t                     height) noexcept
    {
        ND_SET_SCOPE();

        const auto vertexInputStateCreateInfo = PipelineVertexInputStateCreateInfo {
            {{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}},
            {{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)}, {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)}}};

        const auto inputAssemblyStateCreateInfo = getPipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

        const auto tessellationStateCreateInfo = getPipelineTessellationStateCreateInfo(1);

        const auto scissors = VkRect2D {{0, 0}, {width, height}};
        const auto viewport = VkViewport {0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 0.0f};

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

        const auto multisampleStateCreateInfo = getPipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT,
                                                                                      VK_FALSE,
                                                                                      1.0f,
                                                                                      nullptr,
                                                                                      VK_FALSE,
                                                                                      VK_FALSE);

        const auto depthStencilStateCreateInfo = getPipelineDepthStencilStateCreateInfo(VK_FALSE,
                                                                                        VK_FALSE,
                                                                                        VK_COMPARE_OP_NEVER,
                                                                                        VK_FALSE,
                                                                                        VK_FALSE,
                                                                                        {},
                                                                                        {},
                                                                                        0.0,
                                                                                        1.0);

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

        const auto dynamicStateCreateInfo = getPipelineDynamicStateCreateInfo(0, nullptr);

        return {{getMapped<VkShaderModule, VkPipelineShaderStageCreateInfo>(
                     shaderModules,
                     [](const auto& shaderModule, const auto index)
                     {
                         return getPipelineShaderStageCreateInfo(shaderModule.stage, shaderModule.handle, "main", nullptr);
                     }),
                 vertexInputStateCreateInfo,
                 inputAssemblyStateCreateInfo,
                 tessellationStateCreateInfo,
                 {{viewport}, {scissors}},
                 rasterizationStateCreateInfo,
                 multisampleStateCreateInfo,
                 depthStencilStateCreateInfo,
                 {blendConstants, {colorBlendAttachment}, VK_FALSE, VK_LOGIC_OP_OR},
                 dynamicStateCreateInfo,
                 pipelineLayouts[0],
                 renderPass,
                 0}};
    }

    std::vector<CommandPoolConfiguration>
    getCommandPoolConfigurations(const std::vector<QueueFamily>& queueFamiliesPool)
    {
        ND_SET_SCOPE();

        const auto graphicsQueueFamily = std::find_if(queueFamiliesPool.begin(),
                                                      queueFamiliesPool.end(),
                                                      [](const auto& queueFamily)
                                                      {
                                                          return isSubmask(queueFamily.queueFlags, VK_QUEUE_GRAPHICS_BIT);
                                                      });

        ND_ASSERT(graphicsQueueFamily != queueFamiliesPool.end());

        return {{graphicsQueueFamily->index, static_cast<uint32_t>(graphicsQueueFamily->queues.size()), graphicsQueueFamily->queueFlags}};
    }

    std::vector<CommandBufferConfiguration>
    getCommandBufferConfigurations(const std::vector<CommandPool>& commandPools) noexcept
    {
        ND_SET_SCOPE();

        return getMapped<CommandPool, CommandBufferConfiguration>(
            commandPools,
            [](const auto& commandPool, const auto index)
            {
                return CommandBufferConfiguration {commandPool.handle, VK_COMMAND_BUFFER_LEVEL_PRIMARY, commandPool.queueCount};
            });
    }

    std::vector<BufferConfiguration>
    getBufferConfigurations(const std::vector<QueueFamily>& queueFamiliesPool)
    {
        ND_SET_SCOPE();

        const auto graphicsQueueFamily = getQueueFamily(queueFamiliesPool, VK_QUEUE_GRAPHICS_BIT);
        const auto transferQueueFamily = getQueueFamily(queueFamiliesPool, VK_QUEUE_TRANSFER_BIT);

        ND_ASSERT(graphicsQueueFamily != queueFamiliesPool.end());
        ND_ASSERT(transferQueueFamily != queueFamiliesPool.end());

        return {{{graphicsQueueFamily->index},
                 16 * sizeof(Vertex),
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT},
                {{transferQueueFamily->index},
                 16 * sizeof(Vertex),
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT}};
    }
} // namespace nd::src::graphics::vulkan
