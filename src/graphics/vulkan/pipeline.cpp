#include "pipeline.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VkPipelineShaderStageCreateInfo
    getPipelineShaderStageCreateInfo(const VkShaderStageFlagBits            stage,
                                     const VkShaderModule                   shaderModule,
                                     const char*                            name,
                                     const VkSpecializationInfo*            specializationInfo,
                                     const VkPipelineShaderStageCreateFlags flags,
                                     const void*                            next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, // sType;
            next,                                                // pNext;
            flags,                                               // flags;
            stage,                                               // stage;
            shaderModule,                                        // module;
            name,                                                // pName;
            specializationInfo                                   // pSpecializationInfo;
        };
    }

    VkPipelineVertexInputStateCreateInfo
    getPipelineVertexInputStateCreateInfo(const uint32_t                              vertexBindingDescriptionsCount,
                                          const uint32_t                              vertexAttributeDescriptionsCount,
                                          const VkVertexInputBindingDescription*      vertexBindingDescriptions,
                                          const VkVertexInputAttributeDescription*    vertexAttributeDescriptions,
                                          const VkPipelineVertexInputStateCreateFlags flags,
                                          const void*                                 next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO, // sType;
            next,                                                      // pNext;
            flags,                                                     // flags;
            vertexBindingDescriptionsCount,                            // vertexBindingDescriptionCount;
            vertexBindingDescriptions,                                 // pVertexBindingDescriptions;
            vertexAttributeDescriptionsCount,                          // vertexAttributeDescriptionCount;
            vertexAttributeDescriptions                                // pVertexAttributeDescriptions;
        };
    }

    VkPipelineInputAssemblyStateCreateInfo
    getPipelineInputAssemblyStateCreateInfo(const VkPrimitiveTopology                     topology,
                                            const VkBool32                                primitiveRestartEnable,
                                            const VkPipelineInputAssemblyStateCreateFlags flags,
                                            const void*                                   next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, // sType;
            next,                                                        // pNext;
            flags,                                                       // flags;
            topology,                                                    // topology;
            primitiveRestartEnable                                       // primitiveRestartEnable;
        };
    }

    VkPipelineTessellationStateCreateInfo
    getPipelineTessellationStateCreateInfo(const uint32_t                               patchControlPoints,
                                           const VkPipelineTessellationStateCreateFlags flags,
                                           const void*                                  next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO, // sType;
            next,                                                      // pNext;
            flags,                                                     // flags;
            patchControlPoints                                         // patchControlPoints;
        };
    }

    VkPipelineViewportStateCreateInfo
    getPipelineViewportStateCreateInfo(const uint32_t                           viewportsCount,
                                       const uint32_t                           scissorsCount,
                                       const VkViewport*                        viewports,
                                       const VkRect2D*                          scissors,
                                       const VkPipelineViewportStateCreateFlags flags,
                                       const void*                              next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, // sType;
            next,                                                  // pNext;
            flags,                                                 // flags;
            viewportsCount,                                        // viewportCount;
            viewports,                                             // pViewports;
            scissorsCount,                                         // scissorCount;
            scissors                                               // pScissors;
        };
    }

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
                                            const VkPipelineRasterizationStateCreateFlags flags,
                                            const void*                                   next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, // sType;
            next,                                                       // pNext;
            flags,                                                      // flags;
            depthClampEnable,                                           // depthClampEnable;
            rasterizerDiscardEnable,                                    // rasterizerDiscardEnable;
            polygonMode,                                                // polygonMode;
            cullMode,                                                   // cullMode;
            frontFace,                                                  // frontFace;
            depthBiasEnable,                                            // depthBiasEnable;
            depthBiasConstantFactor,                                    // depthBiasConstantFactor;
            depthBiasClamp,                                             // depthBiasClamp;
            depthBiasSlopeFactor,                                       // depthBiasSlopeFactor;
            lineWidth                                                   // lineWidth;
        };
    }

    VkPipelineMultisampleStateCreateInfo
    getPipelineMultisampleStateCreateInfo(const VkSampleCountFlagBits                 rasterizationSamples,
                                          const VkBool32                              sampleShadingEnable,
                                          const float                                 minSampleShading,
                                          const VkSampleMask*                         sampleMask,
                                          const VkBool32                              alphaToCoverageEnable,
                                          const VkBool32                              alphaToOneEnable,
                                          const VkPipelineMultisampleStateCreateFlags flags,
                                          const void*                                 next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO, // sType;
            next,                                                     // pNext;
            flags,                                                    // flags;
            rasterizationSamples,                                     // rasterizationSamples;
            sampleShadingEnable,                                      // sampleShadingEnable;
            minSampleShading,                                         // minSampleShading;
            sampleMask,                                               // pSampleMask;
            alphaToCoverageEnable,                                    // alphaToCoverageEnable;
            alphaToOneEnable                                          // alphaToOneEnable;
        };
    }

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
                                           const VkPipelineDepthStencilStateCreateFlags flags,
                                           const void*                                  next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO, // sType;
            next,                                                       // pNext;
            flags,                                                      // flags;
            depthTestEnable,                                            // depthTestEnable;
            depthWriteEnable,                                           // depthWriteEnable;
            depthCompareOp,                                             // depthCompareOp;
            depthBoundsTestEnable,                                      // depthBoundsTestEnable;
            stencilTestEnable,                                          // stencilTestEnable;
            front,                                                      // front;
            back,                                                       // back;
            minDepthBounds,                                             // minDepthBounds;
            maxDepthBounds                                              // maxDepthBounds;
        };
    }

    VkPipelineColorBlendStateCreateInfo
    getPipelineColorBlendStateCreateInfo(const VkBool32                             logicOpEnable,
                                         const VkLogicOp                            logicOp,
                                         const uint32_t                             attachmentsCount,
                                         const VkPipelineColorBlendAttachmentState* attachments,
                                         const float                                blendConstants[4],
                                         const VkPipelineColorBlendStateCreateFlags flags,
                                         const void*                                next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,                    // sType;
            next,                                                                        // pNext;
            flags,                                                                       // flags;
            logicOpEnable,                                                               // logicOpEnable;
            logicOp,                                                                     // logicOp;
            attachmentsCount,                                                            // attachmentCount;
            attachments,                                                                 // pAttachments;
            {blendConstants[0], blendConstants[1], blendConstants[2], blendConstants[3]} // blendConstants[4];
        };
    }

    VkPipelineDynamicStateCreateInfo
    getPipelineDynamicStateCreateInfo(const uint32_t                          dynamicStatesCount,
                                      const VkDynamicState*                   dynamicStates,
                                      const VkPipelineDynamicStateCreateFlags flags,
                                      const void*                             next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, // sType;
            next,                                                 // pNext;
            flags,                                                // flags;
            dynamicStatesCount,                                   // dynamicStateCount;
            dynamicStates                                         // pDynamicStates;
        };
    }

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
                                  const VkPipelineCreateFlags                   flags,
                                  const void*                                   next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, // sType;
            next,                                            // pNext;
            flags,                                           // flags;
            stagesCount,                                     // stageCount;
            stages,                                          // pStages;
            vertexInputState,                                // pVertexInputState;
            inputAssemblyState,                              // pInputAssemblyState;
            tessellationState,                               // pTessellationState;
            viewportState,                                   // pViewportState;
            rasterizationState,                              // pRasterizationState;
            multisampleState,                                // pMultisampleState;
            depthStencilState,                               // pDepthStencilState;
            colorBlendState,                                 // pColorBlendState;
            dynamicState,                                    // pDynamicState;
            layout,                                          // layout;
            renderPass,                                      // renderPass;
            subpass,                                         // subpass;
            basePipelineHandle,                              // basePipelineHandle;
            basePipelineIndex                                // basePipelineIndex;
        };
    }

    std::vector<VkPipeline>
    getGraphicsPipelineHandle(const std::vector<VkGraphicsPipelineCreateInfo>& createInfos, const VkDevice device)
    {
        ND_SET_SCOPE();

        auto pipelines = std::vector<VkPipeline>(createInfos.size());

        ND_ASSERT_EXEC(
            vkCreateGraphicsPipelines(device, nullptr, pipelines.size(), createInfos.data(), nullptr, pipelines.data()) ==
            VK_SUCCESS);

        return pipelines;
    }

    Pipelines
    getGraphicsPipelines(const std::vector<PipelineConfiguration>& configurations, const VkDevice device)
    {
        ND_SET_SCOPE();

        const auto createInfos = getMapped<PipelineConfiguration, VkGraphicsPipelineCreateInfo>(
            configurations,
            [](const auto& configuration, const auto index)
            {
                return getGraphicsPipelineCreateInfo(configuration.stages.size(),
                                                     configuration.stages.data(),
                                                     configuration.vertexInputState,
                                                     configuration.inputAssemblyState,
                                                     configuration.tessellationState,
                                                     configuration.viewportState,
                                                     configuration.rasterizationState,
                                                     configuration.multisampleState,
                                                     configuration.depthStencilState,
                                                     configuration.colorBlendState,
                                                     configuration.dynamicState,
                                                     configuration.layout,
                                                     configuration.renderPass,
                                                     configuration.subpass,
                                                     VK_NULL_HANDLE,
                                                     0,
                                                     configuration.flags,
                                                     configuration.next);
            });

        return {getGraphicsPipelineHandle(createInfos, device)};
    }
} // namespace nd::src::graphics::vulkan
