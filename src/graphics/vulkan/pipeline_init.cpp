#include "pipeline_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

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
} // namespace nd::src::graphics::vulkan
