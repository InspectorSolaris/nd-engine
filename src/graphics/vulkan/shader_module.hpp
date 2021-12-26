#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct ShaderModuleConfiguration final
    {
        const std::string           path;
        const VkShaderStageFlagBits stage;

        const VkShaderModuleCreateFlags flags = {};
        const void*                     next  = {};
    };

    struct ShaderModule final
    {
        const VkShaderStageFlagBits stage;
        const VkShaderModule        handle;
    };

    std::vector<char>
    getShaderCode(const std::string& path);

    VkShaderModuleCreateInfo
    getShaderModuleCreateInfo(const size_t                    codeSize,
                              const uint32_t*                 code,
                              const VkShaderModuleCreateFlags flags = {},
                              const void*                     next  = {}) noexcept;

    VkShaderModule
    getShaderModuleHandle(const VkShaderModuleCreateInfo& createInfo, const VkDevice device);

    ShaderModule
    getShaderModule(const ShaderModuleConfiguration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
