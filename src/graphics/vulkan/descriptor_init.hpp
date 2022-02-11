#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    DescriptorPool
    createDescriptorPool(opt<const DescriptorPoolCfg>::ref, const VkDevice) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);

    DescriptorSetLayout
    createDescriptorSetLayout(opt<const DescriptorSetLayoutCfg>::ref, const VkDevice) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);

    DescriptorSetLayoutObjects
    createDescriptorSetLayoutObjects(opt<const DescriptorSetLayoutObjectsCfg>::ref, const VkDevice) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);

    vec<DescriptorSet>
    allocateDescriptorSets(opt<const DescriptorSetCfg>::ref, opt<const DescriptorPool>::ref, const VkDevice) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);
} // namespace nd::src::graphics::vulkan
