#include "objects_cfg.hpp"
#include "tools_runtime.hpp"

#if defined(NDEBUG)
    #define ND_VULKAN_DEBUG_LAYERS \
        {                          \
        }
    #define ND_VULKAN_DEBUG_EXTENSIONS \
        {                              \
        }
#else
    #define ND_VULKAN_DEBUG_LAYERS        \
        {                                 \
            "VK_LAYER_KHRONOS_validation" \
        }
    #define ND_VULKAN_DEBUG_EXTENSIONS \
        {                              \
        }
#endif

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    InstanceCfg
    getInstanceCfg(opt<const Dependency>::ref dependency) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.applicationName    = dependency.applicationName,
                .engineName         = dependency.engineName,
                .layers             = getMerged(dependency.layers, ND_VULKAN_DEBUG_LAYERS),
                .extensions         = getMerged(dependency.extensions, ND_VULKAN_DEBUG_EXTENSIONS),
                .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
                .engineVersion      = VK_MAKE_VERSION(0, 0, 1),
                .apiVersion         = VK_API_VERSION_1_2};
    }

    PhysicalDeviceCfg
    getPhysicalDeviceCfg() noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.features = {},
                .priority =
                    [](const auto features, const auto properties)
                {
                    return 1;
                },
                .extensions = {"VK_KHR_swapchain"},
                .queueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT};
    }

    DeviceCfg
    getDeviceCfg(opt<const PhysicalDeviceCfg>::ref physicalDeviceCfg) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.features    = physicalDeviceCfg.features,
                .memory      = {.device = {.size             = 8 * 1024,
                                           .propertyFlags    = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                           .propertyFlagsNot = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT},
                                .host   = {.size             = 8 * 1024,
                                           .propertyFlags    = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                           .propertyFlagsNot = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT}},
                .queueFamily = {.graphics = {.queueFlags = VK_QUEUE_GRAPHICS_BIT, .queueFlagsNot = {}},
                                .transfer = {.queueFlags = VK_QUEUE_TRANSFER_BIT, .queueFlagsNot = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT},
                                .compute  = {.queueFlags = VK_QUEUE_COMPUTE_BIT, .queueFlagsNot = {}}},
                .extensions  = physicalDeviceCfg.extensions,
                .queueFlags  = physicalDeviceCfg.queueFlags};
    }

    BufferObjectsCfg
    getBufferObjectsCfg(opt<const Device>::ref device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto stageSize   = 1024ULL;
        const auto vertexSize  = 1024ULL;
        const auto indexSize   = 1024ULL;
        const auto uniformSize = 1024ULL;

        const auto stageOffset   = 0ULL;
        const auto vertexOffset  = 0ULL;
        const auto indexOffset   = vertexOffset + vertexSize;
        const auto uniformOffset = indexOffset + indexSize;

        const auto meshUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

        const auto stageUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

        return {.mesh  = {.vertex             = {.offset = vertexOffset, .size = vertexSize},
                          .index              = {.offset = indexOffset, .size = indexSize},
                          .uniform            = {.offset = uniformOffset, .size = uniformSize},
                          .queueFamilyIndices = {},
                          .memory             = device.memory.device,
                          .usage              = meshUsage,
                          .sharingMode        = VK_SHARING_MODE_EXCLUSIVE},
                .stage = {.range              = {.offset = stageOffset, .size = stageSize},
                          .queueFamilyIndices = {},
                          .memory             = device.memory.host,
                          .usage              = stageUsage,
                          .sharingMode        = VK_SHARING_MODE_EXCLUSIVE}};
    }

    SwapchainCfg
    getSwapchainCfg(opt<const Dependency>::ref     dependency,
                    opt<const PhysicalDevice>::ref physicalDevice,
                    opt<const Device>::ref         device,
                    opt<const Surface>::ref        surface) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.queueFamily      = device.queueFamily,
                .physicalDevice   = physicalDevice,
                .surface          = surface,
                .imageExtent      = {.width = dependency.width, .height = dependency.height},
                .imageCount       = 4,
                .imageArrayLayers = 1,
                .imageFormat      = VK_FORMAT_B8G8R8A8_UNORM,
                .imageColorSpace  = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
                .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .transform        = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                .presentMode      = VK_PRESENT_MODE_IMMEDIATE_KHR,
                .clipped          = true};
    }

    RenderPassCfg
    getRenderPassCfg(opt<const SwapchainCfg>::ref swapchainCfg) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.attachmentDescriptions = {{.flags          = {},
                                            .format         = swapchainCfg.imageFormat,
                                            .samples        = VK_SAMPLE_COUNT_1_BIT,
                                            .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
                                            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
                                            .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR}},
                .subpassDescriptions    = {{
                       .inputAttachments        = {},
                       .colorAttachments        = {{.attachment = 0U, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}},
                       .resolveAttachments      = {},
                       .depthStencilAttachments = {},
                       .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
                       .flags                   = {},
                }},
                .subpassDependencies    = {}};
    }

    ImageViewCfg
    getSwapchainImageViewCfg(opt<const SwapchainCfg>::ref swapchainCfg) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.subresourceRange =
                    {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0U, .levelCount = 1U, .baseArrayLayer = 0U, .layerCount = 1U},
                .components = {.r = VK_COMPONENT_SWIZZLE_IDENTITY,
                               .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                               .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                               .a = VK_COMPONENT_SWIZZLE_IDENTITY},
                .type       = VK_IMAGE_VIEW_TYPE_2D,
                .format     = swapchainCfg.imageFormat};
    }

    FramebufferCfg
    getSwapchainFramebufferCfg(opt<const SwapchainCfg>::ref swapchainCfg, opt<const RenderPass>::ref renderPass) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.renderPass = renderPass,
                .width      = static_cast<u16>(swapchainCfg.imageExtent.width),
                .height     = static_cast<u16>(swapchainCfg.imageExtent.height),
                .layers     = static_cast<u16>(swapchainCfg.imageArrayLayers)};
    }

    vec<ShaderModuleCfg>
    getShaderModulesCfg() noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {ShaderModuleCfg {.path = "src/graphics/vulkan/shaders/vert.spv", .stage = VK_SHADER_STAGE_VERTEX_BIT},
                ShaderModuleCfg {.path = "src/graphics/vulkan/shaders/frag.spv", .stage = VK_SHADER_STAGE_FRAGMENT_BIT}};
    }

    DescriptorPoolCfg
    getDescriptorPoolCfg() noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.sizes = {VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1U}}, .maxSets = 1U};
    }

    DescriptorSetLayoutObjectsCfg
    getDescriptorSetLayoutObjectsCfg() noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.mesh = DescriptorSetLayoutCfg {}};
    }

    PipelineCacheCfg
    getPipelineCacheCfg() noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {};
    }

    PipelineLayoutObjectsCfg
    getPipelineLayoutObjectsCfg(opt<const DescriptorSetLayoutObjects>::ref descriptorSetLayout) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.mesh = PipelineLayoutCfg {}};
    }

    PipelineObjectsCfg
    getPipelineObjectsCfg(opt<const SwapchainCfg>::ref          swapchainCfg,
                          opt<const RenderPass>::ref            renderPass,
                          opt<const PipelineLayoutObjects>::ref pipelineLayout,
                          const vec<ShaderModule>&              shaderModules) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {
            .mesh = {
                .depthStencil  = {},
                .vertexInput   = {.bindings   = {{.binding = 0U, .stride = 2 * sizeof(glm::vec3), .inputRate = VK_VERTEX_INPUT_RATE_VERTEX}},
                                  .attributes = {{.location = 0U, .binding = 0U, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = 0U},
                                               {.location = 1U, .binding = 0U, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = sizeof(glm::vec3)}}},
                .viewport      = {.viewports = {{.x        = 0.0f,
                                                 .y        = 0.0f,
                                                 .width    = static_cast<float>(swapchainCfg.imageExtent.width),
                                                 .height   = static_cast<float>(swapchainCfg.imageExtent.height),
                                                 .minDepth = 0.0f,
                                                 .maxDepth = 1.0f}},
                                  .scissors  = {{.offset = {.x = 0U, .y = 0U}, .extent = swapchainCfg.imageExtent}}},
                .rasterization = {.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                                  .pNext                   = {},
                                  .flags                   = {},
                                  .depthClampEnable        = VK_FALSE,
                                  .rasterizerDiscardEnable = VK_FALSE,
                                  .polygonMode             = VK_POLYGON_MODE_FILL,
                                  .cullMode                = VK_CULL_MODE_NONE,
                                  .frontFace               = VK_FRONT_FACE_CLOCKWISE,
                                  .depthBiasEnable         = VK_FALSE,
                                  .depthBiasConstantFactor = 0.0f,
                                  .depthBiasClamp          = 0.0f,
                                  .depthBiasSlopeFactor    = 0.0f,
                                  .lineWidth               = 1.0f},
                .colorBlend    = {.attachments    = {{.blendEnable         = VK_TRUE,
                                                      .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
                                                      .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
                                                      .colorBlendOp        = VK_BLEND_OP_ADD,
                                                      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                                                      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                                                      .alphaBlendOp        = VK_BLEND_OP_ADD,
                                                      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                                                    VK_COLOR_COMPONENT_A_BIT}},
                                  .blendConstants = {},
                                  .logicOpEnable  = VK_FALSE,
                                  .logicOp        = {}},
                .multisample   = {.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                                  .pNext                 = {},
                                  .flags                 = {},
                                  .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
                                  .sampleShadingEnable   = VK_FALSE,
                                  .minSampleShading      = 0.0f,
                                  .pSampleMask           = nullptr,
                                  .alphaToCoverageEnable = VK_FALSE,
                                  .alphaToOneEnable      = VK_FALSE},
                .dynamicState  = {},
                .inputAssembly = {.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                                  .pNext                  = {},
                                  .flags                  = {},
                                  .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                                  .primitiveRestartEnable = VK_FALSE},
                .tessellation  = {},
                .stages        = getMapped<ShaderModule, VkPipelineShaderStageCreateInfo>(
                    shaderModules,
                    [](const auto& shaderModule, const auto index)
                    {
                        return VkPipelineShaderStageCreateInfo {.sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                                                                       .pNext               = {},
                                                                       .flags               = {},
                                                                       .stage               = shaderModule.stage,
                                                                       .module              = shaderModule.handle,
                                                                       .pName               = "main",
                                                                       .pSpecializationInfo = {}};
                    }),
                .layout           = pipelineLayout.mesh,
                .renderPass       = renderPass,
                .subpass          = 0,
                .depthStencilUse  = false,
                .vertexInputUse   = true,
                .viewportUse      = true,
                .rasterizationUse = true,
                .colorBlendUse    = true,
                .multisampleUse   = true,
                .dynamicStateUse  = false,
                .inputAssemblyUse = true,
                .tessellationUse  = false}};
    }


    CommandPoolObjectsCfg
    getCommandPoolObjectsCfg(opt<const Device>::ref device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.graphics = {CommandPoolCfg {.queueFamily = device.queueFamily.graphics}},
                .transfer = {CommandPoolCfg {.queueFamily = device.queueFamily.transfer}},
                .compute  = {CommandPoolCfg {.queueFamily = device.queueFamily.compute}}};
    }
} // namespace nd::src::graphics::vulkan
