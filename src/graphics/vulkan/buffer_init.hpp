#pragma once

#include "shared_init.hpp"

#include "memory_init.hpp"

namespace nd::src::graphics::vulkan
{
    BufferMesh
    createBufferMesh(opt<const BufferMeshCfg>::ref, const VkDevice, const VkPhysicalDevice) noexcept(ND_ASSERT_NOTHROW);

    BufferStage
    createBufferStage(opt<const BufferStageCfg>::ref, const VkDevice, const VkPhysicalDevice) noexcept(ND_ASSERT_NOTHROW);

    BufferObjects
    createBufferObjects(opt<const BufferObjectsCfg>::ref, const VkDevice, const VkPhysicalDevice) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
