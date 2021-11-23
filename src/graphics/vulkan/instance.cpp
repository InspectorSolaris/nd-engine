#include "instance.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    Instance::Instance() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    Instance::Instance(const CreateInfo& createInfo)
        : instance_(VK_NULL_HANDLE)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreateInstance(&createInfo, nullptr, &instance_) == VK_SUCCESS);
    }

    Instance::Instance(Instance&& instance) noexcept
        : instance_(std::move(instance.instance_))
    {
        ND_SET_SCOPE_LOW();

        instance.instance_ = VK_NULL_HANDLE;
    }

    Instance&
    Instance::operator=(Instance&& instance) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&instance == this)
        {
            return *this;
        }

        instance_ = std::move(instance.instance_);

        instance.instance_ = VK_NULL_HANDLE;

        return *this;
    }

    Instance::~Instance()
    {
        ND_SET_SCOPE_LOW();

        vkDestroyInstance(instance_, nullptr);
    }

    VkApplicationInfo
    getApplicationInfo(const char*    applicationName,
                       const char*    engineName,
                       const uint32_t applicationVersion,
                       const uint32_t engineVersion,
                       const uint32_t apiVersion) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_APPLICATION_INFO, // sType;
            nullptr,                            // pNext;
            applicationName,                    // pApplicationName;
            applicationVersion,                 // applicationVersion;
            engineName,                         // pEngineName;
            engineVersion,                      // engineVersion;
            apiVersion                          // apiVersion;
        };
    }

    Instance::CreateInfo
    getInstanceCreateInfo(const VkApplicationInfo* applicationInfo,
                          const uint32_t           enabledLayersCount,
                          const uint32_t           enabledExtensionsCount,
                          const char* const*       enabledLayers,
                          const char* const*       enabledExtensions) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, // sType;
            nullptr,                                // pNext;
            0,                                      // flags;
            applicationInfo,                        // pApplicationInfo;
            enabledLayersCount,                     // enabledLayerCount;
            enabledLayers,                          // ppEnabledLayerNames;
            enabledExtensionsCount,                 // enabledExtensionCount;
            enabledExtensions                       // ppEnabledExtensionNames;
        };
    }

    bool
    isInstanceLayersSupported(const Layers& layers) noexcept
    {
        ND_SET_SCOPE_LOW();

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
    isInstanceExtensionsSupported(const Extensions& extensions) noexcept
    {
        ND_SET_SCOPE_LOW();

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

    Instance
    getInstance(const Instance::Configuration& configuration)
    {
        ND_SET_SCOPE_LOW();

        const auto layersSupported     = isInstanceLayersSupported(configuration.layers);
        const auto extensionsSupported = isInstanceExtensionsSupported(configuration.extensions);

        ND_ASSERT(layersSupported && extensionsSupported);

        const auto clayers     = getCStrings(configuration.layers);
        const auto cextensions = getCStrings(configuration.extensions);

        const auto applicationInfo = getApplicationInfo(configuration.applicationName.c_str(),
                                                        configuration.engineName.c_str(),
                                                        configuration.applicationVersion,
                                                        configuration.engineVersion,
                                                        configuration.apiVersion);

        const auto createInfo = getInstanceCreateInfo(&applicationInfo,
                                                      static_cast<uint32_t>(clayers.size()),
                                                      static_cast<uint32_t>(cextensions.size()),
                                                      clayers.data(),
                                                      cextensions.data());

        return Instance(createInfo);
    }
} // namespace nd::src::graphics::vulkan
