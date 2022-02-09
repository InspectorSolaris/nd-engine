#pragma once

#include "pch.hpp"
#include "tools.hpp"

#include "objects.hpp"

namespace nd::src::graphics::vulkan
{
    struct Dependency final
    {
        str applicationName;
        str engineName;

        vec<str> layers;
        vec<str> extensions;

        u16 width;
        u16 height;
    };

    struct InstanceCfg final
    {
        str applicationName;
        str engineName;

        vec<str> layers;
        vec<str> extensions;

        u32 applicationVersion;
        u32 engineVersion;
        u32 apiVersion;

        void*                 instanceNext;
        void*                 applicationNext;
        VkInstanceCreateFlags flags;
    };

    // -------------- SS --------------
    // --------------------------------
    // ------------ DEVICE ------------
    // --------------------------------
    // -------------- SS --------------

    struct PhysicalDeviceCfg final
    {
        VkPhysicalDeviceFeatures features;

        func<u64(const VkPhysicalDeviceFeatures&, const VkPhysicalDeviceProperties&)> priority;

        vec<str> extensions;

        VkQueueFlags queueFlags;
    };

    struct DeviceMemoryCfg final
    {
        VkDeviceSize          size;
        VkMemoryPropertyFlags propertyFlags;
        VkMemoryPropertyFlags propertyFlagsNot;

        void* next;
    };

    struct QueueFamilyCfg final
    {
        VkQueueFlags queueFlags;
        VkQueueFlags queueFlagsNot;
    };

    struct DeviceMemoryObjectsCfg final
    {
        DeviceMemoryCfg device;
        DeviceMemoryCfg host;
    };

    struct QueueFamilyObjectsCfg final
    {
        QueueFamilyCfg graphics;
        QueueFamilyCfg transfer;
        QueueFamilyCfg compute;
    };

    struct DeviceCfg final
    {
        VkPhysicalDeviceFeatures features;

        DeviceMemoryObjectsCfg memory;
        QueueFamilyObjectsCfg  queueFamily;

        vec<str> extensions;

        VkQueueFlags queueFlags;

        void*               next;
        VkDeviceCreateFlags flags;
    };

    // -------------- EE --------------
    // --------------------------------
    // ------------ DEVICE ------------
    // --------------------------------
    // -------------- EE --------------



    // --------------- S ---------------
    // ---------------------------------
    // ------------ BUFFERS ------------
    // ---------------------------------
    // --------------- S ---------------

    struct BufferMeshCfg final
    {
        MemoryRange vertex;
        MemoryRange index;
        MemoryRange uniform;

        gsl::span<const u32> queueFamilyIndices;

        const DeviceMemory& memory;

        VkBufferUsageFlags usage;
        VkSharingMode      sharingMode;

        void*               next;
        VkBufferCreateFlags flags;
    };

    struct BufferStageCfg final
    {
        MemoryRange range;

        gsl::span<const u32> queueFamilyIndices;

        const DeviceMemory& memory;

        VkBufferUsageFlags usage;
        VkSharingMode      sharingMode;

        void*               next;
        VkBufferCreateFlags flags;
    };

    struct BufferObjectsCfg final
    {
        BufferMeshCfg  mesh;
        BufferStageCfg stage;
    };

    // --------------- E ---------------
    // ---------------------------------
    // ------------ BUFFERS ------------
    // ---------------------------------
    // --------------- E ---------------



    // -------------- SS --------------
    // --------------------------------
    // ------------ SCREEN ------------
    // --------------------------------
    // -------------- SS --------------

    struct SwapchainCfg final
    {
        const QueueFamilyObjects& queueFamily;

        VkPhysicalDevice physicalDevice;
        VkSurfaceKHR     surface;

        VkExtent2D imageExtent;

        u32               imageCount;
        u32               imageArrayLayers;
        VkFormat          imageFormat;
        VkColorSpaceKHR   imageColorSpace;
        VkImageUsageFlags imageUsage;
        VkSharingMode     imageSharingMode;

        VkSurfaceTransformFlagBitsKHR transform;
        VkCompositeAlphaFlagBitsKHR   compositeAlpha;
        VkPresentModeKHR              presentMode;

        bool clipped;

        void*                     next;
        VkSwapchainCreateFlagsKHR flags;
    };

    // -------------- EE --------------
    // --------------------------------
    // ------------ SCREEN ------------
    // --------------------------------
    // -------------- EE --------------



    // ----------------- S -----------------
    // -------------------------------------
    // ------------ RENDER PASS ------------
    // -------------------------------------
    // ----------------- S -----------------

    using AttachmentDescription = VkAttachmentDescription;
    using SubpassDependency     = VkSubpassDependency;

    struct SubpassDescription final
    {
        vec<VkAttachmentReference> inputAttachments;
        vec<VkAttachmentReference> colorAttachments;
        vec<VkAttachmentReference> resolveAttachments;
        vec<VkAttachmentReference> depthStencilAttachments;

        vec<u32> preserveAttachments;

        VkPipelineBindPoint       pipelineBindPoint;
        VkSubpassDescriptionFlags flags;
    };

    struct RenderPassCfg final
    {
        vec<AttachmentDescription> attachmentDescriptions;
        vec<SubpassDescription>    subpassDescriptions;
        vec<SubpassDependency>     subpassDependencies;

        void*                   next;
        VkRenderPassCreateFlags flags;
    };

    struct ImageCfg final
    {
    };

    struct ImageViewCfg final
    {
        VkImageSubresourceRange subresourceRange;
        VkComponentMapping      components;
        VkImageViewType         type;
        VkFormat                format;

        void*                  next;
        VkImageViewCreateFlags flags;
    };

    struct FramebufferCfg final
    {
        VkRenderPass renderPass;

        u16 width;
        u16 height;
        u16 layers;

        void*                    next;
        VkFramebufferCreateFlags flags;
    };

    // ----------------- E -----------------
    // -------------------------------------
    // ------------ RENDER PASS ------------
    // -------------------------------------
    // ----------------- E -----------------



    // ----------------- S -----------------
    // -------------------------------------
    // ------------ DESCRIPTORS ------------
    // -------------------------------------
    // ----------------- S -----------------

    struct DescriptorPoolCfg final
    {
        vec<VkDescriptorPoolSize> sizes;

        u16 maxSets;

        void*                       next;
        VkDescriptorPoolCreateFlags flags;
    };

    struct DescriptorSetLayoutCfg final
    {
        vec<VkDescriptorSetLayoutBinding> bindings;

        void*                            next;
        VkDescriptorSetLayoutCreateFlags flags;
    };

    struct DescriptorSetLayoutObjectsCfg final
    {
        DescriptorSetLayoutCfg mesh;
    };

    struct DescriptorSetCfg final
    {
        gsl::span<const DescriptorSetLayout> layouts;

        void* next = {};
    };

    // ----------------- E -----------------
    // -------------------------------------
    // ------------ DESCRIPTORS ------------
    // -------------------------------------
    // ----------------- E -----------------



    // ---------------- S ----------------
    // -----------------------------------
    // ------------ PIPELINES ------------
    // -----------------------------------
    // ---------------- S ----------------

    struct ShaderModuleCfg final
    {
        str path;

        VkShaderStageFlagBits stage;

        void*                     next;
        VkShaderModuleCreateFlags flags;
    };

    struct PipelineCacheCfg final
    {
        void*                      next;
        VkPipelineCacheCreateFlags flags;
    };

    struct PipelineLayoutCfg final
    {
        vec<VkDescriptorSetLayout> descriptorSetLayouts;
        vec<VkPushConstantRange>   pushConstantRanges;

        void*                       next;
        VkPipelineLayoutCreateFlags flags;
    };

    struct PipelineLayoutObjectsCfg final
    {
        PipelineLayoutCfg mesh;
    };

    struct PipelineVertexInputStateCreateInfo final
    {
        vec<VkVertexInputBindingDescription>   bindings;
        vec<VkVertexInputAttributeDescription> attributes;

        void*                                 next;
        VkPipelineVertexInputStateCreateFlags flags;
    };

    struct PipelineViewportStateCreateInfo final
    {
        vec<VkViewport> viewports;
        vec<VkRect2D>   scissors;

        void*                              next;
        VkPipelineViewportStateCreateFlags flags;
    };

    struct PipelineColorBlendStateCreateInfo final
    {
        vec<VkPipelineColorBlendAttachmentState> attachments;
        arr<f32, 4>                              blendConstants;

        VkBool32  logicOpEnable;
        VkLogicOp logicOp;

        void*                                next;
        VkPipelineColorBlendStateCreateFlags flags;
    };

    struct PipelineDynamicStateCreateInfo final
    {
        vec<VkDynamicState> dynamicStates;

        void*                             next;
        VkPipelineDynamicStateCreateFlags flags;
    };

    using PipelineInputAssemblyStateCreateInfo = VkPipelineInputAssemblyStateCreateInfo;
    using PipelineTessellationStateCreateInfo  = VkPipelineTessellationStateCreateInfo;
    using PipelineRasterizationStateCreateInfo = VkPipelineRasterizationStateCreateInfo;
    using PipelineMultisampleStateCreateInfo   = VkPipelineMultisampleStateCreateInfo;
    using PipelineDepthStencilStateCreateInfo  = VkPipelineDepthStencilStateCreateInfo;

    struct GraphicsPipelineContainer final
    {
        VkPipelineVertexInputStateCreateInfo vertexInput;
        VkPipelineViewportStateCreateInfo    viewport;
        VkPipelineColorBlendStateCreateInfo  colorBlend;
        VkPipelineDynamicStateCreateInfo     dynamicState;
    };

    struct GraphicsPipelineCfg final
    {
        PipelineDepthStencilStateCreateInfo  depthStencil;
        PipelineVertexInputStateCreateInfo   vertexInput;
        PipelineViewportStateCreateInfo      viewport;
        PipelineRasterizationStateCreateInfo rasterization;
        PipelineColorBlendStateCreateInfo    colorBlend;
        PipelineMultisampleStateCreateInfo   multisample;
        PipelineDynamicStateCreateInfo       dynamicState;
        PipelineInputAssemblyStateCreateInfo inputAssembly;
        PipelineTessellationStateCreateInfo  tessellation;

        vec<VkPipelineShaderStageCreateInfo> stages;

        VkPipelineLayout layout;
        VkRenderPass     renderPass;

        u32 subpass;

        bool depthStencilUse;
        bool vertexInputUse;
        bool viewportUse;
        bool rasterizationUse;
        bool colorBlendUse;
        bool multisampleUse;
        bool dynamicStateUse;
        bool inputAssemblyUse;
        bool tessellationUse;

        void*                 next;
        VkPipelineCreateFlags flags;
    };

    struct PipelineObjectsCfg final
    {
        GraphicsPipelineCfg mesh;
    };

    // ---------------- E ----------------
    // -----------------------------------
    // ------------ PIPELINES ------------
    // -----------------------------------
    // ---------------- E ----------------



    // --------------- SS ---------------
    // ----------------------------------
    // ------------ COMMANDS ------------
    // ----------------------------------
    // --------------- SS ---------------

    struct CommandPoolCfg final
    {
        const QueueFamily& queueFamily;

        void*                    next;
        VkCommandPoolCreateFlags flags;
    };

    struct CommandPoolObjectsCfg final
    {
        arr<CommandPoolCfg, CommandPoolObjects::graphicsCount> graphics;
        arr<CommandPoolCfg, CommandPoolObjects::transferCount> transfer;
        arr<CommandPoolCfg, CommandPoolObjects::computeCount>  compute;
    };

    struct CommandBufferCfg final
    {
        VkCommandBufferLevel level;
        u16                  count;

        void* next = {};
    };

    // --------------- EE ---------------
    // ----------------------------------
    // ------------ COMMANDS ------------
    // ----------------------------------
    // --------------- EE ---------------

    InstanceCfg getInstanceCfg(opt<const Dependency>::ref) noexcept(ND_ASSERT_NOTHROW);

    PhysicalDeviceCfg
    getPhysicalDeviceCfg() noexcept(ND_ASSERT_NOTHROW);

    DeviceCfg getDeviceCfg(opt<const PhysicalDeviceCfg>::ref) noexcept(ND_ASSERT_NOTHROW);

    BufferObjectsCfg getBufferObjectsCfg(opt<const Device>::ref) noexcept(ND_ASSERT_NOTHROW);

    SwapchainCfg getSwapchainCfg(opt<const Dependency>::ref,
                                 opt<const PhysicalDevice>::ref,
                                 opt<const Device>::ref,
                                 opt<const Surface>::ref) noexcept(ND_ASSERT_NOTHROW);

    RenderPassCfg getRenderPassCfg(opt<const SwapchainCfg>::ref) noexcept(ND_ASSERT_NOTHROW);

    ImageViewCfg getSwapchainImageViewCfg(opt<const SwapchainCfg>::ref) noexcept(ND_ASSERT_NOTHROW);

    FramebufferCfg getSwapchainFramebufferCfg(opt<const SwapchainCfg>::ref, opt<const RenderPass>::ref) noexcept(ND_ASSERT_NOTHROW);

    DescriptorPoolCfg
    getDescriptorPoolCfg() noexcept(ND_ASSERT_NOTHROW);

    DescriptorSetLayoutObjectsCfg
    getDescriptorSetLayoutObjectsCfg() noexcept(ND_ASSERT_NOTHROW);

    vec<ShaderModuleCfg>
    getShaderModulesCfg() noexcept(ND_ASSERT_NOTHROW);

    PipelineCacheCfg
    getPipelineCacheCfg() noexcept(ND_ASSERT_NOTHROW);

    PipelineLayoutObjectsCfg getPipelineLayoutObjectsCfg(opt<const DescriptorSetLayoutObjects>::ref) noexcept(ND_ASSERT_NOTHROW);

    PipelineObjectsCfg
    getPipelineObjectsCfg(opt<const SwapchainCfg>::ref,
                          opt<const RenderPass>::ref,
                          opt<const PipelineLayoutObjects>::ref,
                          const vec<ShaderModule>&) noexcept(ND_ASSERT_NOTHROW);

    CommandPoolObjectsCfg getCommandPoolObjectsCfg(opt<const Device>::ref) noexcept(ND_ASSERT_NOTHROW);

    struct VulkanObjectsCfg final
    {
        using InstanceCfgInit                   = rm_noexcept<decltype(getInstanceCfg)>;
        using PhysicalDeviceCfgInit             = rm_noexcept<decltype(getPhysicalDeviceCfg)>;
        using DeviceCfgInit                     = rm_noexcept<decltype(getDeviceCfg)>;
        using BufferObjectsCfgInit              = rm_noexcept<decltype(getBufferObjectsCfg)>;
        using SwapchainCfgInit                  = rm_noexcept<decltype(getSwapchainCfg)>;
        using RenderPassCfgInit                 = rm_noexcept<decltype(getRenderPassCfg)>;
        using SwapchainImageViewCfgInit         = rm_noexcept<decltype(getSwapchainImageViewCfg)>;
        using SwapchainFramebufferCfgInit       = rm_noexcept<decltype(getSwapchainFramebufferCfg)>;
        using DescriptorPoolCfgInit             = rm_noexcept<decltype(getDescriptorPoolCfg)>;
        using DescriptorSetLayoutObjectsCfgInit = rm_noexcept<decltype(getDescriptorSetLayoutObjectsCfg)>;
        using ShaderModulesCfgInit              = rm_noexcept<decltype(getShaderModulesCfg)>;
        using PipelineCacheCfgInit              = rm_noexcept<decltype(getPipelineCacheCfg)>;
        using PipelineLayoutObjectsCfgInit      = rm_noexcept<decltype(getPipelineLayoutObjectsCfg)>;
        using PipelineObjectsCfgInit            = rm_noexcept<decltype(getPipelineObjectsCfg)>;
        using CommandPoolObjectsCfgInit         = rm_noexcept<decltype(getCommandPoolObjectsCfg)>;

        func<InstanceCfgInit>                   instance;
        func<PhysicalDeviceCfgInit>             physicalDevice;
        func<DeviceCfgInit>                     device;
        func<BufferObjectsCfgInit>              buffer;
        func<SwapchainCfgInit>                  swapchain;
        func<RenderPassCfgInit>                 renderPass;
        func<SwapchainImageViewCfgInit>         swapchainImageView;
        func<SwapchainFramebufferCfgInit>       swapchainFramebuffer;
        func<DescriptorPoolCfgInit>             descriptorPool;
        func<DescriptorSetLayoutObjectsCfgInit> descriptorSetLayout;
        func<ShaderModulesCfgInit>              shaderModules;
        func<PipelineCacheCfgInit>              pipelineCache;
        func<PipelineLayoutObjectsCfgInit>      pipelineLayout;
        func<PipelineObjectsCfgInit>            pipeline;
        func<CommandPoolObjectsCfgInit>         commandPool;
    };
} // namespace nd::src::graphics::vulkan
