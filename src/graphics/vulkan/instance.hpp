#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class Instance final
    {
    public:
        struct Configuration final
        {
            const std::string& applicationName;
            const std::string& engineName;

            const std::vector<std::string>& layers;
            const std::vector<std::string>& extensions;

            const uint32_t applicationVersion;
            const uint32_t engineVersion;
            const uint32_t apiVersion;
        };

        Instance() noexcept;
        Instance(const VkInstanceCreateInfo& createInfo);

        Instance(const Instance& instance) = delete;
        Instance(Instance&& instance) noexcept;

        Instance&
        operator=(const Instance& instance) = delete;
        Instance&
        operator=(Instance&& instance) noexcept;

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
    getApplicationInfo(const char*    applicationName,
                       const char*    engineName,
                       const uint32_t applicationVersion,
                       const uint32_t engineVersion,
                       const uint32_t apiVersion) noexcept;

    VkInstanceCreateInfo
    getInstanceCreateInfo(const VkApplicationInfo*    applicationInfo,
                          const uint32_t              enabledLayersCount,
                          const uint32_t              enabledExtensionsCount,
                          const char* const*          enabledLayers,
                          const char* const*          enabledExtensions,
                          const VkInstanceCreateFlags flags = {},
                          const void*                 next  = {}) noexcept;

    bool
    isInstanceLayersSupported(const std::vector<std::string>& layers) noexcept;

    bool
    isInstanceExtensionsSupported(const std::vector<std::string>& extensions) noexcept;

    Instance
    getInstance(const Instance::Configuration& configuration);
} // namespace nd::src::graphics::vulkan
