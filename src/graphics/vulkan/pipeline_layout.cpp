#include "pipeline_layout.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VkPipelineLayoutCreateInfo
    getPipelineLayoutCreateInfo(const uint32_t                    setLayoutsCount,
                                const uint32_t                    pushConstantRangesCount,
                                const VkDescriptorSetLayout*      setLayouts,
                                const VkPushConstantRange*        pushConstantRanges,
                                const VkPipelineLayoutCreateFlags flags,
                                const void*                       next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, // sType;
            next,                                          // pNext;
            flags,                                         // flags;
            setLayoutsCount,                               // setLayoutCount;
            setLayouts,                                    // pSetLayouts;
            pushConstantRangesCount,                       // pushConstantRangeCount;
            pushConstantRanges                             // pPushConstantRanges;
        };
    }

    VkPipelineLayoutCreateInfo
    getPipelineLayoutCreateInfo(const PipelineLayoutConfiguration& configuration)
    {
        ND_SET_SCOPE();

        return getPipelineLayoutCreateInfo(configuration.descriptorSetLayouts.size(),
                                           configuration.pushConstantRanges.size(),
                                           configuration.descriptorSetLayouts.data(),
                                           configuration.pushConstantRanges.data());
    }

    VkPipelineLayout
    getPipelineLayout(const VkPipelineLayoutCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkPipelineLayout pipelineLayout;

        ND_ASSERT(vkCreatePipelineLayout(device, &createInfo, nullptr, &pipelineLayout) == VK_SUCCESS);

        return pipelineLayout;
    }
} // namespace nd::src::graphics::vulkan
