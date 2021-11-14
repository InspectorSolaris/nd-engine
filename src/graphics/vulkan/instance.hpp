#pragma occurrences

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class Instance final
    {
    public:
        using CreateInfo = VkInstanceCreateInfo;
        using Layer      = std::string;
        using Layers     = std::vector<Layer>;
        using Extension  = std::string;
        using Extensions = std::vector<Extension>;

        struct Configuration final
        {
            const std::string applicationName;
            const std::string engineName;

            const Layers     layers;
            const Extensions extensions;

            const uint32_t applicationVersion;
            const uint32_t engineVersion;
            const uint32_t apiVersion;
        };

        Instance() noexcept;
        Instance(const CreateInfo &createInfo);

        Instance(const Instance &instance) = delete;
        Instance(Instance &&instance) noexcept;

        Instance &
        operator=(const Instance &instance) = delete;
        Instance &
        operator=(Instance &&instance) noexcept;

        ~Instance();

        constexpr VkInstance
        get() const noexcept;

    private:
        VkInstance instance_ {VK_NULL_HANDLE};
    };

    constexpr VkInstance
    Instance::get() const noexcept
    {
        return instance_;
    }

    VkApplicationInfo
    getApplicationInfo(const char *   applicationName,
                       const char *   engineName,
                       const uint32_t applicationVersion,
                       const uint32_t engineVersion,
                       const uint32_t apiVersion) noexcept;

    Instance::CreateInfo
    getInstanceCreateInfo(const VkApplicationInfo *applicationInfo,
                          const uint32_t           enabledLayersCount,
                          const uint32_t           enabledExtensionsCount,
                          const char *const *      enabledLayers,
                          const char *const *      enabledExtensions) noexcept;

    bool
    isInstanceLayersSupported(const Instance::Layers &layers) noexcept;

    bool
    isInstanceExtensionsSupported(const Instance::Extensions &extensions) noexcept;

    Instance
    getInstance(const Instance::Configuration &configuration);
}    // namespace nd::src::graphics::vulkan
