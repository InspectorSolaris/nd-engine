#include "pipeline_layout.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    PipelineLayout::PipelineLayout() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    PipelineLayout::PipelineLayout(const VkDevice device, const CreateInfo& createInfo)
        : device_(device)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreatePipelineLayout(device_, &createInfo, nullptr, &pipelineLayout_) == VK_SUCCESS);
    }

    PipelineLayout::PipelineLayout(PipelineLayout&& pipelineLayout) noexcept
        : device_(std::move(pipelineLayout.device_))
        , pipelineLayout_(std::move(pipelineLayout.pipelineLayout_))
    {
        ND_SET_SCOPE_LOW();

        pipelineLayout.pipelineLayout_ = VK_NULL_HANDLE;
    }

    PipelineLayout&
    PipelineLayout::operator=(PipelineLayout&& pipelineLayout) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&pipelineLayout == this)
        {
            return *this;
        }

        device_         = std::move(pipelineLayout.device_);
        pipelineLayout_ = std::move(pipelineLayout.pipelineLayout_);

        pipelineLayout.pipelineLayout_ = VK_NULL_HANDLE;

        return *this;
    }

    PipelineLayout::~PipelineLayout()
    {
        ND_SET_SCOPE_LOW();

        vkDestroyPipelineLayout(device_, pipelineLayout_, nullptr);
    }

    PipelineLayout::CreateInfo
    getPipelineLayoutCreateInfo(const uint32_t               setLayoutsCount,
                                const uint32_t               pushConstantRangesCount,
                                const VkDescriptorSetLayout* setLayouts,
                                const VkPushConstantRange*   pushConstantRanges) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, // sType;
            nullptr,                                       // pNext;
            0,                                             // flags;
            setLayoutsCount,                               // setLayoutCount;
            setLayouts,                                    // pSetLayouts;
            pushConstantRangesCount,                       // pushConstantRangeCount;
            pushConstantRanges                             // pPushConstantRanges;
        };
    }

    PipelineLayout
    getPipelineLayout(const PipelineLayout::Configuration& configuration, const VkDevice device)
    {
        ND_SET_SCOPE_LOW();

        const auto createInfo = getPipelineLayoutCreateInfo(configuration.descriptorSetLayouts.size(),
                                                            configuration.pushConstantRanges.size(),
                                                            configuration.descriptorSetLayouts.data(),
                                                            configuration.pushConstantRanges.data());

        return PipelineLayout(device, createInfo);
    }
} // namespace nd::src::graphics::vulkan
