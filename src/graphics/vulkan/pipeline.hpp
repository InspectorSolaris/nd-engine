#pragma once

#include "pch.hpp"
#include "shared.hpp"

#include "shader_module.hpp"

namespace nd::src::graphics::vulkan
{
    class Pipeline final
    {
    public:
        using CreateInfo    = VkGraphicsPipelineCreateInfo;
        using ShaderModules = std::vector<ShaderModule>;

        struct Configuration final
        {
        };

        Pipeline() noexcept;
        Pipeline(const VkDevice device, const CreateInfo& createInfo);

        Pipeline(const Pipeline& pipeline) = delete;
        Pipeline(Pipeline&& pipeline) noexcept;

        Pipeline&
        operator=(const Pipeline& pipeline) = delete;
        Pipeline&
        operator=(Pipeline&& pipeline) noexcept;

        ~Pipeline();

        constexpr VkPipeline
        get() const noexcept;

    private:
        VkDevice   device_ {VK_NULL_HANDLE};
        VkPipeline pipeline_ {VK_NULL_HANDLE};
    };

    constexpr VkPipeline
    Pipeline::get() const noexcept
    {
        return pipeline_;
    }

    VkPipelineVertexInputStateCreateInfo
    getPipelineVertexInputStateCreateInfo(const uint32_t                           vertexBindingDescriptionsCount,
                                          const uint32_t                           vertexAttributeDescriptionsCount,
                                          const VkVertexInputBindingDescription*   vertexBindingDescriptions,
                                          const VkVertexInputAttributeDescription* vertexAttributeDescriptions) noexcept;

    VkPipelineInputAssemblyStateCreateInfo
    getPipelineInputAssemblyStateCreateInfo(const VkPrimitiveTopology topology,
                                            const VkBool32            primitiveRestartEnable) noexcept;

    VkPipelineTessellationStateCreateInfo
    getPipelineTessellationStateCreateInfo(const uint32_t patchControlPoints) noexcept;

    VkPipelineViewportStateCreateInfo
    getPipelineViewportStateCreateInfo(const uint32_t    viewportsCount,
                                       const uint32_t    scissorsCount,
                                       const VkViewport* viewports,
                                       const VkRect2D*   scissors) noexcept;

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
                                            const float           lineWidth) noexcept;

    VkPipelineMultisampleStateCreateInfo
    getPipelineMultisampleStateCreateInfo(const VkSampleCountFlagBits rasterizationSamples,
                                          const VkBool32              sampleShadingEnable,
                                          const float                 minSampleShading,
                                          const VkSampleMask*         sampleMask,
                                          const VkBool32              alphaToCoverageEnable,
                                          const VkBool32              alphaToOneEnable) noexcept;

    VkPipelineDepthStencilStateCreateInfo
    getPipelineDepthStencilStateCreateInfo(const VkBool32         depthTestEnable,
                                           const VkBool32         depthWriteEnable,
                                           const VkCompareOp      depthCompareOp,
                                           const VkBool32         depthBoundsTestEnable,
                                           const VkBool32         stencilTestEnable,
                                           const VkStencilOpState front,
                                           const VkStencilOpState back,
                                           const float            minDepthBounds,
                                           const float            maxDepthBounds) noexcept;

    VkPipelineColorBlendStateCreateInfo
    getPipelineColorBlendStateCreateInfo(const VkBool32                             logicOpEnable,
                                         const VkLogicOp                            logicOp,
                                         const uint32_t                             attachmentsCount,
                                         const VkPipelineColorBlendAttachmentState* attachments,
                                         const float                                blendConstants[4]) noexcept;

    VkPipelineDynamicStateCreateInfo
    getPipelineDynamicStateCreateInfo(const uint32_t dynamicStatesCount, const VkDynamicState* dynamicStates) noexcept;

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
                                  const int32_t                                 basePipelineIndex) noexcept;

    Pipeline
    getGraphicsPipeline(const Pipeline::Configuration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
