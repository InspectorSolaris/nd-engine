#pragma once

#include "shared_init.hpp"

#include "memory_init.hpp"
#include "queue_init.hpp"

namespace nd::src::graphics::vulkan
{
    vec<VkPhysicalDevice>
    getPhysicalDevices(const VkInstance) noexcept;

    vec<VkExtensionProperties>
    getPhysicalDeviceExtensionProperties(const VkPhysicalDevice) noexcept;

    vec<VkQueueFamilyProperties>
    getPhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice) noexcept;

    vec<QueueFamily>
    getPhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice, const VkQueueFlags) noexcept;

    VkPhysicalDeviceFeatures
    getPhysicalDeviceFeatures(const VkPhysicalDevice) noexcept;

    VkPhysicalDeviceProperties
    getPhysicalDeviceProperties(const VkPhysicalDevice) noexcept;

    VkPhysicalDeviceMemoryProperties
    getPhysicalDeviceMemoryProperties(const VkPhysicalDevice) noexcept;

    bool
    isPhysicalDeviceExtensionsSupported(const VkPhysicalDevice, const vec<str>&) noexcept;

    bool
    isPhysicalDeviceFeaturesSupported(const VkPhysicalDevice, const VkPhysicalDeviceFeatures&) noexcept;

    bool
    isPhysicalDeviceQueuesSupported(const VkPhysicalDevice, const VkQueueFlags) noexcept;

    PhysicalDevice
    getPhysicalDevice(opt<const PhysicalDeviceCfg>::ref, const VkInstance) noexcept(ND_ASSERT_NOTHROW);

    Device
    createDevice(opt<const DeviceCfg>::ref, const VkPhysicalDevice) noexcept(ND_ASSERT_NOTHROW);
} // namespace nd::src::graphics::vulkan
