#include "instance.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VkApplicationInfo
    getApplicationInfo(const char*    applicationName,
                       const char*    engineName,
                       const uint32_t applicationVersion,
                       const uint32_t engineVersion,
                       const uint32_t apiVersion,
                       const void*    next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_APPLICATION_INFO, // sType;
            next,                               // pNext;
            applicationName,                    // pApplicationName;
            applicationVersion,                 // applicationVersion;
            engineName,                         // pEngineName;
            engineVersion,                      // engineVersion;
            apiVersion                          // apiVersion;
        };
    }

    VkInstanceCreateInfo
    getInstanceCreateInfo(const VkApplicationInfo*    applicationInfo,
                          const uint32_t              enabledLayersCount,
                          const uint32_t              enabledExtensionsCount,
                          const char* const*          enabledLayers,
                          const char* const*          enabledExtensions,
                          const VkInstanceCreateFlags flags,
                          const void*                 next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, // sType;
            next,                                   // pNext;
            flags,                                  // flags;
            applicationInfo,                        // pApplicationInfo;
            enabledLayersCount,                     // enabledLayerCount;
            enabledLayers,                          // ppEnabledLayerNames;
            enabledExtensionsCount,                 // enabledExtensionCount;
            enabledExtensions                       // ppEnabledExtensionNames;
        };
    }

    bool
    isInstanceLayersSupported(const std::vector<std::string>& layers) noexcept
    {
        ND_SET_SCOPE();

        uint32_t layersCount;

        vkEnumerateInstanceLayerProperties(&layersCount, nullptr);

        auto layersProperties = std::vector<VkLayerProperties>(layersCount);

        vkEnumerateInstanceLayerProperties(&layersCount, layersProperties.data());

        for(const auto& layer: layers)
        {
            if(std::none_of(layersProperties.begin(),
                            layersProperties.end(),
                            [&layer](const auto& layerProperties)
                            {
                                return std::strcmp(layer.c_str(), layerProperties.layerName) == 0;
                            }))
            {
                return false;
            }
        }

        return true;
    }

    bool
    isInstanceExtensionsSupported(const std::vector<std::string>& extensions) noexcept
    {
        ND_SET_SCOPE();

        uint32_t extensionsCount;

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);

        auto extensionsProperties = std::vector<VkExtensionProperties>(extensionsCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, extensionsProperties.data());

        for(const auto& extension: extensions)
        {
            if(std::none_of(extensionsProperties.begin(),
                            extensionsProperties.end(),
                            [&extension](const auto& extensionProperties)
                            {
                                return std::strcmp(extension.c_str(), extensionProperties.extensionName) == 0;
                            }))
            {
                return false;
            }
        }

        return true;
    }

    VkInstance
    getInstanceHandle(const VkInstanceCreateInfo& createInfo)
    {
        ND_SET_SCOPE();

        VkInstance instance;

        ND_ASSERT_EXEC(vkCreateInstance(&createInfo, nullptr, &instance) == VK_SUCCESS);

        return instance;
    }

    Instance
    getInstance(const InstanceConfiguration& configuration)
    {
        ND_SET_SCOPE();

        const auto layersSupported     = isInstanceLayersSupported(configuration.layers);
        const auto extensionsSupported = isInstanceExtensionsSupported(configuration.extensions);

        ND_ASSERT(layersSupported && extensionsSupported);

        const auto clayers     = getRawStrings(configuration.layers);
        const auto cextensions = getRawStrings(configuration.extensions);

        const auto applicationInfo = getApplicationInfo(configuration.applicationName.c_str(),
                                                        configuration.engineName.c_str(),
                                                        configuration.applicationVersion,
                                                        configuration.engineVersion,
                                                        configuration.apiVersion);

        const auto createInfo = getInstanceCreateInfo(&applicationInfo,
                                                      static_cast<uint32_t>(clayers.size()),
                                                      static_cast<uint32_t>(cextensions.size()),
                                                      clayers.data(),
                                                      cextensions.data(),
                                                      configuration.flags,
                                                      configuration.next);

        return {getInstanceHandle(createInfo)};
    }
} // namespace nd::src::graphics::vulkan
