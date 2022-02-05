#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    DescriptorPool
    createDescriptorPool(opt<const DescriptorPoolCfg>::ref, const VkDevice) noexcept(ND_ASSERT_NOTHROW);

    DescriptorSetLayout
    createDescriptorSetLayout(opt<const DescriptorSetLayoutCfg>::ref, const VkDevice) noexcept(ND_ASSERT_NOTHROW);

    DescriptorSetLayoutObjects
    createDescriptorSetLayoutObjects(opt<const DescriptorSetLayoutObjectsCfg>::ref, const VkDevice) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
