#include "pipeline.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    Pipeline::Pipeline() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    Pipeline::Pipeline(const VkDevice device, const CreateInfo& createInfo)
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
    getPipelineShaderStageCreateInfo(const VkShaderStageFlagBits stage,
                                     const VkShaderModule        shaderModule,
                                     const char*                 name,
                                     const VkSpecializationInfo* specializationInfo) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, // sType;
            nullptr,                                             // pNext;
            0,                                                   // flags;
            stage,                                               // stage;
            shaderModule,                                        // module;
            name,                                                // pName;
            specializationInfo                                   // pSpecializationInfo;
        };
    }

    VkPipelineVertexInputStateCreateInfo
    getPipelineVertexInputStateCreateInfo(const uint32_t                           vertexBindingDescriptionsCount,
                                          const uint32_t                           vertexAttributeDescriptionsCount,
                                          const VkVertexInputBindingDescription*   vertexBindingDescriptions,
                                          const VkVertexInputAttributeDescription* vertexAttributeDescriptions) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO, // sType;
            nullptr,                                                   // pNext;
            0,                                                         // flags;
            vertexBindingDescriptionsCount,                            // vertexBindingDescriptionCount;
            vertexBindingDescriptions,                                 // pVertexBindingDescriptions;
            vertexAttributeDescriptionsCount,                          // vertexAttributeDescriptionCount;
            vertexAttributeDescriptions                                // pVertexAttributeDescriptions;
        };
    }

    VkPipelineInputAssemblyStateCreateInfo
    getPipelineInputAssemblyStateCreateInfo(const VkPrimitiveTopology topology,
                                            const VkBool32            primitiveRestartEnable) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, // sType;
            nullptr,                                                     // pNext;
            0,                                                           // flags;
            topology,                                                    // topology;
            primitiveRestartEnable                                       // primitiveRestartEnable;
        };
    }

    VkPipelineTessellationStateCreateInfo
    getPipelineTessellationStateCreateInfo(const uint32_t patchControlPoints) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO, // sType;
            nullptr,                                                   // pNext;
            0,                                                         // flags;
            patchControlPoints                                         // patchControlPoints;
        };
    }

    VkPipelineViewportStateCreateInfo
    getPipelineViewportStateCreateInfo(const uint32_t    viewportsCount,
                                       const uint32_t    scissorsCount,
                                       const VkViewport* viewports,
                                       const VkRect2D*   scissors) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, // sType;
            nullptr,                                               // pNext;
            0,                                                     // flags;
            viewportsCount,                                        // viewportCount;
            viewports,                                             // pViewports;
            scissorsCount,                                         // scissorCount;
            scissors                                               // pScissors;
        };
    }

    VkPipelineRasterizationStateCreateInfo
    getPipelineRasterizationStateCreateInfo(const VkBool32        depthClampEnable,
                                            const VkBool32        rasterizerDiscardEnable,
                                            const VkPolygonMode   polygonMode,
                                            const VkCullModeFlags cullMode,
                                            const VkFrontFace     frontFace,
                                            const VkBool32        depthBiasEnable,
                                            const float           depthBiasConstantFactor,
                                            const float           depthBiasClamp,
                                            const float           depthBiasSlopeFactor,
                                            const float           lineWidth) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, // sType;
            nullptr,                                         // pNext;
            0,                                               // flags;
            depthClampEnable,                                // depthClampEnable;
            rasterizerDiscardEnable,                         // rasterizerDiscardEnable;
            polygonMode,                                     // polygonMode;
            cullMode,                                        // cullMode;
            frontFace,                                       // frontFace;
            depthBiasEnable,                                 // depthBiasEnable;
            depthBiasConstantFactor,                         // depthBiasConstantFactor;
            depthBiasClamp,                                  // depthBiasClamp;
            depthBiasSlopeFactor,                            // depthBiasSlopeFactor;
            lineWidth                                        // lineWidth;
        };
    }

    VkPipelineMultisampleStateCreateInfo
    getPipelineMultisampleStateCreateInfo(const VkSampleCountFlagBits rasterizationSamples,
                                          const VkBool32              sampleShadingEnable,
                                          const float                 minSampleShading,
                                          const VkSampleMask*         sampleMask,
                                          const VkBool32              alphaToCoverageEnable,
                                          const VkBool32              alphaToOneEnable) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO, // sType;
            nullptr,                                                  // pNext;
            0,                                                        // flags;
            rasterizationSamples,                                     // rasterizationSamples;
            sampleShadingEnable,                                      // sampleShadingEnable;
            minSampleShading,                                         // minSampleShading;
            sampleMask,                                               // pSampleMask;
            alphaToCoverageEnable,                                    // alphaToCoverageEnable;
            alphaToOneEnable                                          // alphaToOneEnable;
        };
    }

    VkPipelineDepthStencilStateCreateInfo
    getPipelineDepthStencilStateCreateInfo(const VkBool32         depthTestEnable,
                                           const VkBool32         depthWriteEnable,
                                           const VkCompareOp      depthCompareOp,
                                           const VkBool32         depthBoundsTestEnable,
                                           const VkBool32         stencilTestEnable,
                                           const VkStencilOpState front,
                                           const VkStencilOpState back,
                                           const float            minDepthBounds,
                                           const float            maxDepthBounds) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO, // sType;
            nullptr,                                                    // pNext;
            0,                                                          // flags;
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
                                         const float                                blendConstants[4]) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO, // sType;
            nullptr, // pNext;
            0, // flags;
            logicOpEnable, // logicOpEnable;
            logicOp, // logicOp;
            attachmentsCount, // attachmentCount;
            attachments, // pAttachments;
            {blendConstants[0], blendConstants[1], blendConstants[2], blendConstants[3]}  // blendConstants[4];
        };
    }

    VkPipelineDynamicStateCreateInfo
    getPipelineDynamicStateCreateInfo(const uint32_t dynamicStatesCount, const VkDynamicState* dynamicStates) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, // sType;
            nullptr,                                              // pNext;
            0,                                                    // flags;
            dynamicStatesCount,                                   // dynamicStateCount;
            dynamicStates                                         // pDynamicStates;
        };
    }

    Pipeline::CreateInfo
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
                                  const int32_t                                 basePipelineIndex) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, // sType;
            nullptr,                                         // pNext;
            0,                                               // flags;
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
} // namespace nd::src::graphics::vulkan
