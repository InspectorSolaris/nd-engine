#pragma once

#include "pch.hpp"
#include "shared.hpp"

#include "queue.hpp"

namespace nd::src::graphics::vulkan
{
    struct PhysicalDeviceConfiguration final
    {
        using PhysicalDevicePriority = size_t(const VkPhysicalDevice,
                                              const VkPhysicalDeviceProperties&,
                                              const VkPhysicalDeviceFeatures&);

        const VkPhysicalDeviceFeatures features;

        const std::function<PhysicalDevicePriority> priority;

        const std::vector<std::string> extensions;

        const VkQueueFlags queueFlags;
    };

    struct DeviceConfiguration final
    {
        const VkPhysicalDeviceFeatures features;

        const std::vector<std::string> extensions;

        const VkDeviceCreateFlags flags = {};
        const void*               next  = {};
    };

    struct Device final
    {
        const std::map<uint32_t, std::vector<VkQueue>> queues;
        const std::vector<QueueFamily>                 queueFamilies;

        const VkDevice handle;
    };

    VkDeviceQueueCreateInfo
    getQueueCreateInfo(const uint32_t                 queueFamilyIndex,
                       const uint32_t                 queueCount,
                       const float*                   queuePriorities,
                       const VkDeviceQueueCreateFlags flags = {},
                       const void*                    next  = {}) noexcept;

    VkDeviceCreateInfo
    getDeviceCreateInfo(const uint32_t                  queueCreateInfosCount,
                        const uint32_t                  enabledExtensionsCount,
                        const VkDeviceQueueCreateInfo*  queueCreateInfos,
                        const char* const*              enabledExtensions,
                        const VkPhysicalDeviceFeatures* enabledFeatures,
                        const VkDeviceCreateFlags       flags = {},
                        const void*                     next  = {}) noexcept;

    std::vector<VkPhysicalDevice>
    getPhysicalDevices(const VkInstance instance) noexcept;

    bool
    isPhysicalDeviceFeaturesSupported(const VkPhysicalDeviceFeatures* available,
                                      const VkPhysicalDeviceFeatures* required) noexcept;

    bool
    isPhysicalDeviceExtensionsSupported(const VkPhysicalDevice          physicalDevice,
                                        const std::vector<std::string>& extensions) noexcept;

    bool
    isPhysicalDeviceQueuesSupported(const VkPhysicalDevice physicalDevice, const VkQueueFlags queueFlags) noexcept;

    VkPhysicalDevice
    getPhysicalDevice(const PhysicalDeviceConfiguration& configuration, const VkInstance instance);

    VkDevice
    getDeviceHandle(const VkDeviceCreateInfo& createInfo, const VkPhysicalDevice physicalDevice);

    Device
    getDevice(const DeviceConfiguration& configuration, const VkPhysicalDevice physicalDevice);
} // namespace nd::src::graphics::vulkan
