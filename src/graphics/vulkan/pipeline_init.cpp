#include "pipeline_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VkGraphicsPipelineCreateInfo
    getGraphicsPipelineCreateInfo(opt<const GraphicsPipelineCfg>::ref cfg, GraphicsPipelineContainer& container) noexcept
    {
        ND_SET_SCOPE();

        container.vertexInput = {.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
                                 .pNext                           = cfg.vertexInput.next,
                                 .flags                           = cfg.vertexInput.flags,
                                 .vertexBindingDescriptionCount   = static_cast<u32>(cfg.vertexInput.bindings.size()),
                                 .pVertexBindingDescriptions      = cfg.vertexInput.bindings.data(),
                                 .vertexAttributeDescriptionCount = static_cast<u32>(cfg.vertexInput.attributes.size()),
                                 .pVertexAttributeDescriptions    = cfg.vertexInput.attributes.data()};

        container.viewport = {.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                              .pNext         = cfg.viewport.next,
                              .flags         = cfg.viewport.flags,
                              .viewportCount = static_cast<u32>(cfg.viewport.viewports.size()),
                              .pViewports    = cfg.viewport.viewports.data(),
                              .scissorCount  = static_cast<u32>(cfg.viewport.scissors.size()),
                              .pScissors     = cfg.viewport.scissors.data()};

        container.colorBlend = {.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                                .pNext           = cfg.colorBlend.next,
                                .flags           = cfg.colorBlend.flags,
                                .logicOpEnable   = cfg.colorBlend.logicOpEnable,
                                .logicOp         = cfg.colorBlend.logicOp,
                                .attachmentCount = static_cast<u32>(cfg.colorBlend.attachments.size()),
                                .pAttachments    = cfg.colorBlend.attachments.data(),
                                .blendConstants  = {cfg.colorBlend.blendConstants[0],
                                                   cfg.colorBlend.blendConstants[1],
                                                   cfg.colorBlend.blendConstants[2],
                                                   cfg.colorBlend.blendConstants[3]}};

        container.dynamicState = {.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                                  .pNext             = cfg.dynamicState.next,
                                  .flags             = cfg.dynamicState.flags,
                                  .dynamicStateCount = static_cast<u32>(cfg.dynamicState.dynamicStates.size()),
                                  .pDynamicStates    = cfg.dynamicState.dynamicStates.data()};

        return VkGraphicsPipelineCreateInfo {.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                                             .pNext               = cfg.next,
                                             .flags               = cfg.flags,
                                             .stageCount          = static_cast<u32>(cfg.stages.size()),
                                             .pStages             = cfg.stages.data(),
                                             .pVertexInputState   = cfg.vertexInputUse ? &container.vertexInput : nullptr,
                                             .pInputAssemblyState = cfg.inputAssemblyUse ? &cfg.inputAssembly : nullptr,
                                             .pTessellationState  = cfg.tessellationUse ? &cfg.tessellation : nullptr,
                                             .pViewportState      = cfg.viewportUse ? &container.viewport : nullptr,
                                             .pRasterizationState = cfg.rasterizationUse ? &cfg.rasterization : nullptr,
                                             .pMultisampleState   = cfg.multisampleUse ? &cfg.multisample : nullptr,
                                             .pDepthStencilState  = cfg.depthStencilUse ? &cfg.depthStencil : nullptr,
                                             .pColorBlendState    = cfg.colorBlendUse ? &container.colorBlend : nullptr,
                                             .pDynamicState       = cfg.dynamicStateUse ? &container.dynamicState : nullptr,
                                             .layout              = cfg.layout,
                                             .renderPass          = cfg.renderPass,
                                             .subpass             = cfg.subpass,
                                             .basePipelineHandle  = VK_NULL_HANDLE,
                                             .basePipelineIndex   = -1};
    }

    PipelineCache
    createPipelineCache(opt<const PipelineCacheCfg>::ref cfg, const VkDevice device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto createInfo = VkPipelineCacheCreateInfo {.sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
                                                           .pNext           = cfg.next,
                                                           .flags           = cfg.flags,
                                                           .initialDataSize = 0ULL,
                                                           .pInitialData    = nullptr};

        VkPipelineCache pipelineCache;

        ND_ASSERT_EXEC(vkCreatePipelineCache(device, &createInfo, ND_VULKAN_ALLOCATION_CALLBACKS, &pipelineCache) == VK_SUCCESS);

        return pipelineCache;
    }

    PipelineLayout
    createPipelineLayout(opt<const PipelineLayoutCfg>::ref cfg, const VkDevice device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto createInfo = VkPipelineLayoutCreateInfo {.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                                                            .pNext                  = cfg.next,
                                                            .flags                  = cfg.flags,
                                                            .setLayoutCount         = static_cast<u32>(cfg.descriptorSetLayouts.size()),
                                                            .pSetLayouts            = cfg.descriptorSetLayouts.data(),
                                                            .pushConstantRangeCount = static_cast<u32>(cfg.pushConstantRanges.size()),
                                                            .pPushConstantRanges    = cfg.pushConstantRanges.data()};

        VkPipelineLayout pipelineLayout;

        ND_ASSERT_EXEC(vkCreatePipelineLayout(device, &createInfo, ND_VULKAN_ALLOCATION_CALLBACKS, &pipelineLayout) == VK_SUCCESS);

        return pipelineLayout;
    }

    PipelineLayoutObjects
    createPipelineLayoutObjects(opt<const PipelineLayoutObjectsCfg>::ref cfg, const VkDevice device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.mesh = createPipelineLayout(cfg.mesh, device)};
    }

    PipelineObjects
    createPipelineObjects(opt<const PipelineObjectsCfg>::ref cfg,
                          const VkDevice                     device,
                          const VkPipelineCache              pipelineCache) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        auto graphicsContainers = std::array<GraphicsPipelineContainer, 1> {};

        auto graphicsCreateInfos = std::array {getGraphicsPipelineCreateInfo(cfg.mesh, graphicsContainers[0])};

        auto graphicsPipelines = std::array<VkPipeline, graphicsCreateInfos.size()> {};

        ND_ASSERT_EXEC(vkCreateGraphicsPipelines(device,
                                                 pipelineCache,
                                                 graphicsCreateInfos.size(),
                                                 graphicsCreateInfos.data(),
                                                 ND_VULKAN_ALLOCATION_CALLBACKS,
                                                 graphicsPipelines.data()) == VK_SUCCESS);

        return {.mesh = graphicsPipelines[0]};
    }
} // namespace nd::src::graphics::vulkan
