#pragma once

#include "shared_init.hpp"

namespace nd::src::graphics::vulkan
{
    vec<char>
    getShaderModuleCode(const str_v path) noexcept(ND_ASSERT_NOTHROW);

    ShaderModule
    createShaderModule(opt<const ShaderModuleCfg>::ref, const VkDevice) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);

    vec<ShaderModule>
    createShaderModules(const span<const ShaderModuleCfg>, const VkDevice) noexcept(ND_VK_ASSERT_EXEC_NOTHROW);
} // namespace nd::src::graphics::vulkan
