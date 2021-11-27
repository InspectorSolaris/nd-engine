#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class ShaderModule final
    {
    public:
        using CreateInfo = VkShaderModuleCreateInfo;
        using Code       = std::vector<char>;

        struct Configuration final
        {
            const std::string&          path;
            const VkShaderStageFlagBits stage;
        };

        ShaderModule() noexcept;
        ShaderModule(const VkDevice device, const VkShaderStageFlagBits stage, const CreateInfo& createInfo);

        ShaderModule(const ShaderModule& shaderModule) = delete;
        ShaderModule(ShaderModule&& shaderModule) noexcept;

        ShaderModule&
        operator=(const ShaderModule& shaderModule) = delete;
        ShaderModule&
        operator=(ShaderModule&& shaderModule) noexcept;

        ~ShaderModule();

        constexpr VkShaderModule
        get() const noexcept;

        constexpr VkShaderStageFlagBits
        getStage() const noexcept;

    private:
        VkDevice       device_ {VK_NULL_HANDLE};
        VkShaderModule shaderModule_ {VK_NULL_HANDLE};

        VkShaderStageFlagBits stage_ {};
    };

    constexpr VkShaderModule
    ShaderModule::get() const noexcept
    {
        return shaderModule_;
    }

    constexpr VkShaderStageFlagBits
    ShaderModule::getStage() const noexcept
    {
        return stage_;
    }

    ShaderModule::Code
    getShaderCode(const std::string& path);

    ShaderModule::CreateInfo
    getShaderModuleCreateInfo(const size_t codeSize, const uint32_t* code) noexcept;

    ShaderModule
    getShaderModule(const ShaderModule::Configuration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
