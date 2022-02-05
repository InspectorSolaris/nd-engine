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
} // namespace nd::src::graphics::vulkan
