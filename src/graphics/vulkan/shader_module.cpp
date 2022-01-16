#include "shader_module.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    std::vector<char>
    getShaderCode(const std::string& path)
    {
        ND_SET_SCOPE();

        ND_ASSERT(std::filesystem::exists(path));

        auto file = std::ifstream(path, std::ios::ate | std::ios::binary);

        ND_ASSERT(file);

        auto size = static_cast<size_t>(file.tellg());
        auto code = std::vector<char>(size);

        file.seekg(0);
        file.read(code.data(), size);

        return code;
    }

    VkShaderModuleCreateInfo
    getShaderModuleCreateInfo(const size_t codeSize, const uint32_t* code, const VkShaderModuleCreateFlags flags, const void* next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, // sType;
            next,                                        // pNext;
            flags,                                       // flags;
            codeSize,                                    // codeSize;
            code                                         // pCode;
        };
    }

    VkShaderModule
    getShaderModuleHandle(const VkShaderModuleCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkShaderModule shaderModule;

        ND_ASSERT_EXEC(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) == VK_SUCCESS);

        return shaderModule;
    }

    ShaderModule
    getShaderModule(const ShaderModuleConfiguration& configuration, const VkDevice device)
    {
        ND_SET_SCOPE();

        const auto code = getShaderCode(configuration.path);

        const auto createInfo = getShaderModuleCreateInfo(code.size(),
                                                          reinterpret_cast<const uint32_t*>(code.data()),
                                                          configuration.flags,
                                                          configuration.next);

        return {configuration.stage, getShaderModuleHandle(createInfo, device)};
    }

    std::vector<ShaderModule>
    getShaderModules(const std::vector<ShaderModuleConfiguration>& configurations, const VkDevice device)
    {
        ND_SET_SCOPE();

        return getMapped<ShaderModuleConfiguration, ShaderModule>(configurations,
                                                                  [device](const auto& configuration, const auto index)
                                                                  {
                                                                      return getShaderModule(configuration, device);
                                                                  });
    }
} // namespace nd::src::graphics::vulkan
