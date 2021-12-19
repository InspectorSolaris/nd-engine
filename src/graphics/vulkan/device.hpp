#pragma once

#include "pch.hpp"
#include "shared.hpp"

#include "queue.hpp"

namespace nd::src::graphics::vulkan
{
    struct DeviceConfiguration final
    {
        const VkPhysicalDeviceFeatures& features;

        const std::function<size_t(const VkPhysicalDevice,
                                   const VkPhysicalDeviceProperties&,
                                   const VkPhysicalDeviceFeatures&)>& physicalDevicePriority;

        const std::vector<std::string>& extensions;

        const VkQueueFlags queueFlags;
    };

    struct DeviceCreateInfo
    {
        const VkDeviceCreateInfo       createInfo;
        const std::vector<QueueFamily> queueFamilies;
        const VkPhysicalDevice         physicalDevice;
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

    std::vector<VkQueueFamilyProperties>
    getPhysicalDeviceQueueFamilies(const VkPhysicalDevice physicalDevice) noexcept;

    std::vector<QueueFamily>
    getDeviceQueueFamilies(const VkPhysicalDevice physicalDevice, const VkQueueFlags queueFlags) noexcept;

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
    getPhysicalDevice(const DeviceConfiguration& configuration, const VkInstance instance);

    DeviceCreateInfo
    getDeviceCreateInfo(const DeviceConfiguration& configuration, const VkInstance instance);

    VkDevice
    getDevice(const VkDeviceCreateInfo& createInfo, const VkPhysicalDevice physicalDevice);
} // namespace nd::src::graphics::vulkan
