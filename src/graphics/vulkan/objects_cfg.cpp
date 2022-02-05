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

        return {.features        = physicalDeviceCfg.features,
                .memoryPool      = {.device = {.size             = 1024,
                                               .propertyFlags    = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                               .propertyFlagsNot = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT},
                                    .host   = {.size             = 1024,
                                               .propertyFlags    = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                               .propertyFlagsNot = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT}},
                .queueFamilyPool = {.graphics = {.queueFlags = VK_QUEUE_GRAPHICS_BIT, .queueFlagsNot = {}},
                                    .transfer = {.queueFlags = VK_QUEUE_TRANSFER_BIT, .queueFlagsNot = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT},
                                    .compute  = {.queueFlags = VK_QUEUE_COMPUTE_BIT, .queueFlagsNot = {}}},
                .extensions      = physicalDeviceCfg.extensions,
                .queueFlags      = physicalDeviceCfg.queueFlags};
    }

    SwapchainCfg
    getSwapchainCfg(opt<const Dependency>::ref     dependency,
                    opt<const PhysicalDevice>::ref physicalDevice,
                    opt<const Device>::ref         device,
                    opt<const Surface>::ref        surface) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.queueFamilyPool  = device.queueFamilyPool,
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

        return {.attachmentDescriptions = {AttachmentDescription {.flags          = {},
                                                                  .format         = swapchainCfg.imageFormat,
                                                                  .samples        = VK_SAMPLE_COUNT_1_BIT,
                                                                  .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                                  .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
                                                                  .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                                  .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                                  .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
                                                                  .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR}},
                .subpassDescriptions    = {SubpassDescription {
                       .inputAttachments        = {},
                       .colorAttachments        = {VkAttachmentReference {.attachment = 0U, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}},
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
    getShaderModuleCfgs() noexcept(ND_ASSERT_NOTHROW)
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

    DescriptorSetLayoutPoolCfg
    getDescriptorSetLayoutPoolCfg() noexcept(ND_ASSERT_NOTHROW)
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

    PipelineLayoutPoolCfg
    getPipelineLayoutPoolCfg(opt<const DescriptorSetLayoutPool>::ref descriptorSetLayoutPool) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.mesh = PipelineLayoutCfg {}};
    }

    PipelinePoolCfg
    getPipelinePoolCfg(opt<const RenderPass>::ref renderPass, opt<const PipelineLayoutPool>::ref pipelineLayoutPool) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {};
    }
} // namespace nd::src::graphics::vulkan
