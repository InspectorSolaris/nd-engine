#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct InstanceConfiguration final
    {
        const std::string& applicationName;
        const std::string& engineName;

        const std::vector<std::string>& layers;
        const std::vector<std::string>& extensions;

        const uint32_t applicationVersion;
        const uint32_t engineVersion;
        const uint32_t apiVersion;
    };

    VkApplicationInfo
    getApplicationInfo(const char*    applicationName,
                       const char*    engineName,
                       const uint32_t applicationVersion,
                       const uint32_t engineVersion,
                       const uint32_t apiVersion,
                       const void*    next = {}) noexcept;

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

    VkInstance
    getInstance(const VkInstanceCreateInfo& createInfo);

    VkInstance
    getInstance(const InstanceConfiguration& configuration, const VkInstanceCreateFlags flags = {}, const void* next = {});
} // namespace nd::src::graphics::vulkan
