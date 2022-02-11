#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    PipelineCache
    createPipelineCache(opt<const PipelineCacheCfg>::ref, const VkDevice) noexcept(ND_VK_ASSERT_NOTHROW);

    PipelineLayout
    createPipelineLayout(opt<const PipelineLayoutCfg>::ref, const VkDevice) noexcept(ND_VK_ASSERT_NOTHROW);

    PipelineLayoutObjects
    createPipelineLayoutObjects(opt<const PipelineLayoutObjectsCfg>::ref, const VkDevice) noexcept(ND_VK_ASSERT_NOTHROW);

    PipelineObjects
    createPipelineObjects(opt<const PipelineObjectsCfg>::ref cfg,
                          const VkDevice                     device,
                          const VkPipelineCache              pipelineCache) noexcept(ND_VK_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
