#include "pipeline.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    Pipeline::Pipeline() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    Pipeline::Pipeline(const VkDevice device, const VkGraphicsPipelineCreateInfo& createInfo)
        : device_(device)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreateGraphicsPipelines(device_, nullptr, 1, &createInfo, nullptr, &pipeline_) == VK_SUCCESS);
    }

    Pipeline::Pipeline(Pipeline&& pipeline) noexcept
        : device_(std::move(pipeline.device_))
        , pipeline_(std::move(pipeline.pipeline_))
    {
        ND_SET_SCOPE_LOW();

        pipeline.pipeline_ = VK_NULL_HANDLE;
    }

    Pipeline&
    Pipeline::operator=(Pipeline&& pipeline) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&pipeline == this)
        {
            return *this;
        }

        device_   = std::move(pipeline.device_);
        pipeline_ = std::move(pipeline.pipeline_);

        pipeline.pipeline_ = VK_NULL_HANDLE;

        return *this;
    }

    Pipeline::~Pipeline()
    {
        ND_SET_SCOPE_LOW();

        vkDestroyPipeline(device_, pipeline_, nullptr);
    }

    VkPipelineShaderStageCreateInfo
    getPipelineShaderStageCreateInfo(const VkShaderStageFlagBits            stage,
                                     const VkShaderModule                   shaderModule,
                                     const char*                            name,
                                     const VkSpecializationInfo*            specializationInfo,
                                     const VkPipelineShaderStageCreateFlags flags,
                                     const void*                            next) noexcept
    {
        ND_SET_SCOPE_LOW();

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
        ND_SET_SCOPE_LOW();

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
        ND_SET_SCOPE_LOW();

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
        ND_SET_SCOPE_LOW();

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
        ND_SET_SCOPE_LOW();

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
        ND_SET_SCOPE_LOW();

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
        ND_SET_SCOPE_LOW();

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
        ND_SET_SCOPE_LOW();

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
        ND_SET_SCOPE_LOW();

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
        ND_SET_SCOPE_LOW();

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
        ND_SET_SCOPE_LOW();

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

    Pipeline
    getGraphicsPipeline(const Pipeline::Configuration& configuration, const VkDevice device)
    {
        ND_SET_SCOPE_LOW();

        const auto shaderStages = getMapped<Pipeline::ShaderInfo, VkPipelineShaderStageCreateInfo>(
            configuration.shaderInfos,
            [](const auto& shaderInfo)
            {
                return getPipelineShaderStageCreateInfo(shaderInfo.shaderStage, shaderInfo.shaderModule, "main", nullptr);
            });

        const auto vertexInputState = getPipelineVertexInputStateCreateInfo(0, 0, nullptr, nullptr);
        const auto inputAssemblyState =
            getPipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

        const auto tessellationState = VkPipelineTessellationStateCreateInfo {};

        const auto scissors = VkRect2D {{0, 0}, {configuration.width, configuration.height}};
        const auto viewport = VkViewport {0.0f,
                                          0.0f,
                                          static_cast<float>(configuration.width),
                                          static_cast<float>(configuration.height),
                                          0.0f,
                                          0.1f};

        const auto viewportState = getPipelineViewportStateCreateInfo(1, 1, &viewport, &scissors);

        const auto rasterizationState = getPipelineRasterizationStateCreateInfo(VK_FALSE,
                                                                                VK_FALSE,
                                                                                VK_POLYGON_MODE_FILL,
                                                                                VK_CULL_MODE_NONE,
                                                                                VK_FRONT_FACE_CLOCKWISE,
                                                                                VK_FALSE,
                                                                                0.0f,
                                                                                0.0f,
                                                                                0.0f,
                                                                                1.0f);

        const auto multisampleState =
            getPipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f, nullptr, VK_FALSE, VK_FALSE);

        const auto depthStencilState = VkPipelineDepthStencilStateCreateInfo {};

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

        const auto colorBlendState =
            getPipelineColorBlendStateCreateInfo(VK_FALSE, VK_LOGIC_OP_OR, 1, &colorBlendAttachment, blendConstants.data());

        const auto dynamicState = getPipelineDynamicStateCreateInfo(0, nullptr);

        const auto createInfo = getGraphicsPipelineCreateInfo(shaderStages.size(),
                                                              shaderStages.data(),
                                                              &vertexInputState,
                                                              &inputAssemblyState,
                                                              nullptr,
                                                              &viewportState,
                                                              &rasterizationState,
                                                              &multisampleState,
                                                              nullptr,
                                                              &colorBlendState,
                                                              &dynamicState,
                                                              configuration.pipelineLayout,
                                                              configuration.renderPass,
                                                              configuration.subpass,
                                                              VK_NULL_HANDLE,
                                                              0);

        return Pipeline(device, createInfo);
    }
} // namespace nd::src::graphics::vulkan
