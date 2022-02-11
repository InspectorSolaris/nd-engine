#include "instance_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    vec<VkLayerProperties>
    getInstanceLayerProperties() noexcept(ND_VK_ASSERT_EXEC_NOTHROW)
    {
        ND_SET_SCOPE();

        u32 count;

        ND_VK_ASSERT_EXEC(vkEnumerateInstanceLayerProperties(&count, nullptr));

        auto properties = vec<VkLayerProperties>(count);

        ND_VK_ASSERT_EXEC(vkEnumerateInstanceLayerProperties(&count, properties.data()));

        return properties;
    }

    vec<VkExtensionProperties>
    getInstanceExtensionProperties() noexcept(ND_VK_ASSERT_EXEC_NOTHROW)
    {
        ND_SET_SCOPE();

        u32 count;

        ND_VK_ASSERT_EXEC(vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr));

        auto properties = vec<VkExtensionProperties>(count);

        ND_VK_ASSERT_EXEC(vkEnumerateInstanceExtensionProperties(nullptr, &count, properties.data()));

        return properties;
    }

    bool
    isInstanceLayersSupported(const vec<str>& layers) noexcept(ND_VK_ASSERT_EXEC_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto instanceLayers = getInstanceLayerProperties();

        return std::all_of(layers.begin(),
                           layers.end(),
                           [&instanceLayers](const auto& layer)
                           {
                               return std::any_of(instanceLayers.begin(),
                                                  instanceLayers.end(),
                                                  [&layer](const auto& instanceLayer)
                                                  {
                                                      return !std::strcmp(layer.c_str(), instanceLayer.layerName);
                                                  });
                           });
    }

    bool
    isInstanceExtensionsSupported(const vec<str>& extensions) noexcept(ND_VK_ASSERT_EXEC_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto instanceExtensions = getInstanceExtensionProperties();

        return std::all_of(extensions.begin(),
                           extensions.end(),
                           [&instanceExtensions](const auto& extension)
                           {
                               return std::any_of(instanceExtensions.begin(),
                                                  instanceExtensions.end(),
                                                  [&extension](const auto& instanceExtension)
                                                  {
                                                      return !std::strcmp(extension.c_str(), instanceExtension.extensionName);
                                                  });
                           });
    }

    Instance
    createInstance(opt<const InstanceCfg>::ref cfg) noexcept(ND_VK_ASSERT_EXEC_NOTHROW&& ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        ND_ASSERT(isInstanceLayersSupported(cfg.layers));
        ND_ASSERT(isInstanceExtensionsSupported(cfg.extensions));

        const auto applicationInfo = VkApplicationInfo {.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                                        .pNext              = cfg.applicationNext,
                                                        .pApplicationName   = cfg.applicationName.c_str(),
                                                        .applicationVersion = cfg.applicationVersion,
                                                        .pEngineName        = cfg.engineName.c_str(),
                                                        .engineVersion      = cfg.engineVersion,
                                                        .apiVersion         = cfg.apiVersion};

        const auto clayers     = getRawStrings(cfg.layers);
        const auto cextensions = getRawStrings(cfg.extensions);

        const auto clayersSize     = static_cast<u32>(clayers.size());
        const auto cextensionsSize = static_cast<u32>(cextensions.size());

        const auto createInfo = VkInstanceCreateInfo {.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                                      .pNext                   = cfg.instanceNext,
                                                      .flags                   = cfg.flags,
                                                      .pApplicationInfo        = &applicationInfo,
                                                      .enabledLayerCount       = clayersSize,
                                                      .ppEnabledLayerNames     = clayers.data(),
                                                      .enabledExtensionCount   = cextensionsSize,
                                                      .ppEnabledExtensionNames = cextensions.data()};

        VkInstance instance;

        ND_VK_ASSERT_EXEC(vkCreateInstance(&createInfo, ND_VK_ALLOCATION_CALLBACKS, &instance));

        return instance;
    }
} // namespace nd::src::graphics::vulkan
