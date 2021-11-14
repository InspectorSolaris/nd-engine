#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    class ShaderModule final
    {
    public:
        using CreateInfo = VkShaderModuleCreateInfo;

        ShaderModule() noexcept;
        ShaderModule(const VkDevice device, const CreateInfo& createInfo);

        ShaderModule(const ShaderModule& shaderModule) = delete;
        ShaderModule(ShaderModule&& shaderModule) noexcept;

        ShaderModule&
        operator=(const ShaderModule& shaderModule) = delete;
        ShaderModule&
        operator=(ShaderModule&& shaderModule) noexcept;

        ~ShaderModule();

        constexpr VkShaderModule
        get() const noexcept;

    private:
        VkDevice       device_ {VK_NULL_HANDLE};
        VkShaderModule shaderModule_ {VK_NULL_HANDLE};
    };

    constexpr VkShaderModule
    ShaderModule::get() const noexcept
    {
        return shaderModule_;
    }

    ShaderModule::CreateInfo
    getShaderModuleCreateInfo(const size_t codeSize, const uint32_t* code) noexcept;
} // namespace nd::src::graphics::vulkan
