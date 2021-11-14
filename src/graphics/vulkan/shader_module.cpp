#include "shader_module.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    ShaderModule::ShaderModule() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    ShaderModule::ShaderModule(const VkDevice device, const CreateInfo &createInfo) : device_(device)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreateShaderModule(device_, &createInfo, nullptr, &shaderModule_) == VK_SUCCESS);
    }

    ShaderModule::ShaderModule(ShaderModule &&shaderModule) noexcept :
        device_(std::move(shaderModule.device_)),
        shaderModule_(std::move(shaderModule.shaderModule_))
    {
        ND_SET_SCOPE_LOW();

        shaderModule.shaderModule_ = VK_NULL_HANDLE;
    }

    ShaderModule &
    ShaderModule::operator=(ShaderModule &&shaderModule) noexcept
    {
        ND_SET_SCOPE_LOW();

        if (&shaderModule == this)
        {
            return *this;
        }

        device_       = std::move(shaderModule.device_);
        shaderModule_ = std::move(shaderModule.shaderModule_);

        shaderModule.shaderModule_ = VK_NULL_HANDLE;

        return *this;
    }

    ShaderModule::~ShaderModule()
    {
        ND_SET_SCOPE_LOW();

        vkDestroyShaderModule(device_, shaderModule_, nullptr);
    }

    ShaderModule::CreateInfo
    getShaderModuleCreateInfo(const size_t codeSize, const uint32_t *code) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,    // sType;
            nullptr,                                        // pNext;
            0,                                              // flags;
            codeSize,                                       // codeSize;
            code                                            // pCode;
        };
    }
}    // namespace nd::src::graphics::vulkan
