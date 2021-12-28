#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    struct PipelineConfiguration final
    {
        const std::vector<VkPipelineShaderStageCreateInfo>& stages;

        const VkPipelineVertexInputStateCreateInfo*   vertexInputState;
        const VkPipelineInputAssemblyStateCreateInfo* inputAssemblyState;
        const VkPipelineTessellationStateCreateInfo*  tessellationState;
        const VkPipelineViewportStateCreateInfo*      viewportState;
        const VkPipelineRasterizationStateCreateInfo* rasterizationState;
        const VkPipelineMultisampleStateCreateInfo*   multisampleState;
        const VkPipelineDepthStencilStateCreateInfo*  depthStencilState;
        const VkPipelineColorBlendStateCreateInfo*    colorBlendState;
        const VkPipelineDynamicStateCreateInfo*       dynamicState;
        const VkPipelineLayout                        layout;
        const VkRenderPass                            renderPass;
        const uint32_t                                subpass;

        const VkPipelineCreateFlags flags = {};
        const void*                 next  = {};
    };

    struct Pipelines final
    {
        const std::vector<VkPipeline> handles;
    };

    VkPipelineShaderStageCreateInfo
    getPipelineShaderStageCreateInfo(const VkShaderStageFlagBits            stage,
                                     const VkShaderModule                   shaderModule,
                                     const char*                            name,
                                     const VkSpecializationInfo*            specializationInfo,
                                     const VkPipelineShaderStageCreateFlags flags = {},
                                     const void*                            next  = {}) noexcept;

    VkPipelineVertexInputStateCreateInfo
    getPipelineVertexInputStateCreateInfo(const uint32_t                              vertexBindingDescriptionsCount,
                                          const uint32_t                              vertexAttributeDescriptionsCount,
                                          const VkVertexInputBindingDescription*      vertexBindingDescriptions,
                                          const VkVertexInputAttributeDescription*    vertexAttributeDescriptions,
                                          const VkPipelineVertexInputStateCreateFlags flags = {},
                                          const void*                                 next  = {}) noexcept;

    VkPipelineInputAssemblyStateCreateInfo
    getPipelineInputAssemblyStateCreateInfo(const VkPrimitiveTopology                     topology,
                                            const VkBool32                                primitiveRestartEnable,
                                            const VkPipelineInputAssemblyStateCreateFlags flags = {},
                                            const void*                                   next  = {}) noexcept;

    VkPipelineTessellationStateCreateInfo
    getPipelineTessellationStateCreateInfo(const uint32_t                               patchControlPoints,
                                           const VkPipelineTessellationStateCreateFlags flags = {},
                                           const void*                                  next  = {}) noexcept;

    VkPipelineViewportStateCreateInfo
    getPipelineViewportStateCreateInfo(const uint32_t                           viewportsCount,
                                       const uint32_t                           scissorsCount,
                                       const VkViewport*                        viewports,
                                       const VkRect2D*                          scissors,
                                       const VkPipelineViewportStateCreateFlags flags = {},
                                       const void*                              next  = {}) noexcept;

    VkPipelineRasterizationStateCreateInfo
    getPipelineRasterizationStateCreateInfo(const VkBool32                                depthClampEnable,
                                            const VkBool32                                rasterizerDiscardEnable,
                                            const VkPolygonMode                           polygonMode,
                                            const VkCullModeFlags                         cullMode,
                                            const VkFrontFace                             frontFace,
                                            const VkBool32                                depthBiasEnable,
                                            const float                                   depthBiasConstantFactor,
                                            const float                                   depthBiasClamp,
                                            const float                                   depthBiasSlopeFactor,
                                            const float                                   lineWidth,
                                            const VkPipelineRasterizationStateCreateFlags flags = {},
                                            const void*                                   next  = {}) noexcept;

    VkPipelineMultisampleStateCreateInfo
    getPipelineMultisampleStateCreateInfo(const VkSampleCountFlagBits                 rasterizationSamples,
                                          const VkBool32                              sampleShadingEnable,
                                          const float                                 minSampleShading,
                                          const VkSampleMask*                         sampleMask,
                                          const VkBool32                              alphaToCoverageEnable,
                                          const VkBool32                              alphaToOneEnable,
                                          const VkPipelineMultisampleStateCreateFlags flags = {},
                                          const void*                                 next  = {}) noexcept;

    VkPipelineDepthStencilStateCreateInfo
    getPipelineDepthStencilStateCreateInfo(const VkBool32                               depthTestEnable,
                                           const VkBool32                               depthWriteEnable,
                                           const VkCompareOp                            depthCompareOp,
                                           const VkBool32                               depthBoundsTestEnable,
                                           const VkBool32                               stencilTestEnable,
                                           const VkStencilOpState                       front,
                                           const VkStencilOpState                       back,
                                           const float                                  minDepthBounds,
                                           const float                                  maxDepthBounds,
                                           const VkPipelineDepthStencilStateCreateFlags flags = {},
                                           const void*                                  next  = {}) noexcept;

    VkPipelineColorBlendStateCreateInfo
    getPipelineColorBlendStateCreateInfo(const VkBool32                             logicOpEnable,
                                         const VkLogicOp                            logicOp,
                                         const uint32_t                             attachmentsCount,
                                         const VkPipelineColorBlendAttachmentState* attachments,
                                         const float                                blendConstants[4],
                                         const VkPipelineColorBlendStateCreateFlags flags = {},
                                         const void*                                next  = {}) noexcept;

    VkPipelineDynamicStateCreateInfo
    getPipelineDynamicStateCreateInfo(const uint32_t                          dynamicStatesCount,
                                      const VkDynamicState*                   dynamicStates,
                                      const VkPipelineDynamicStateCreateFlags flags = {},
                                      const void*                             next  = {}) noexcept;

    VkGraphicsPipelineCreateInfo
    getGraphicsPipelineCreateInfo(const uint32_t                                stagesCount,
                                  const VkPipelineShaderStageCreateInfo*        stages,
                                  const VkPipelineVertexInputStateCreateInfo*   vertexInputState,
                                  const VkPipelineInputAssemblyStateCreateInfo* inputAssemblyState,
                                  const VkPipelineTessellationStateCreateInfo*  tessellationState,
                                  const VkPipelineViewportStateCreateInfo*      viewportState,
                                  const VkPipelineRasterizationStateCreateInfo* rasterizationState,
                                  const VkPipelineMultisampleStateCreateInfo*   multisampleState,
                                  const VkPipelineDepthStencilStateCreateInfo*  depthStencilState,
                                  const VkPipelineColorBlendStateCreateInfo*    colorBlendState,
                                  const VkPipelineDynamicStateCreateInfo*       dynamicState,
                                  const VkPipelineLayout                        layout,
                                  const VkRenderPass                            renderPass,
                                  const uint32_t                                subpass,
                                  const VkPipeline                              basePipelineHandle,
                                  const int32_t                                 basePipelineIndex,
                                  const VkPipelineCreateFlags                   flags = {},
                                  const void*                                   next  = {}) noexcept;

    std::vector<VkPipeline>
    getGraphicsPipelineHandle(const std::vector<VkGraphicsPipelineCreateInfo>& createInfos, const VkDevice device);

    Pipelines
    getGraphicsPipelines(const std::vector<PipelineConfiguration>& configurations, const VkDevice device);
} // namespace nd::src::graphics::vulkan
