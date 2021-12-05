#pragma once

#include "pch.hpp"
#include "shared.hpp"

#include "queue.hpp"

namespace nd::src::graphics::vulkan
{
    struct PhysicalDevice final
    {
        VkPhysicalDevice           handle;
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures   features;
    };

    class Device final
    {
    public:
        struct Configuration final
        {
            const VkPhysicalDeviceFeatures& features;

            const std::function<size_t(const PhysicalDevice&)>& physicalDevicePriority;

            const std::vector<std::string>& extensions;

            const VkQueueFlags queueFlags;
        };

        Device() noexcept;
        Device(const VkPhysicalDevice          physicalDevice,
               const std::vector<QueueFamily>& queueFamilies,
               const VkDeviceCreateInfo&       createInfo);

        Device(const Device& device) = delete;
        Device(Device&& device) noexcept;

        Device&
        operator=(const Device& device) = delete;
        Device&
        operator=(Device&& device) noexcept;

        ~Device();

        constexpr VkPhysicalDevice
        getPhysical() const noexcept;

        constexpr VkDevice
        get() const noexcept;

        constexpr const std::vector<QueueFamily>&
        getQueueFamilies() const noexcept;

    private:
        std::vector<QueueFamily> queueFamilies_ {};

        VkPhysicalDevice physicalDevice_ {VK_NULL_HANDLE};
        VkDevice         device_ {VK_NULL_HANDLE};
    };

    constexpr VkPhysicalDevice
    Device::getPhysical() const noexcept
    {
        return physicalDevice_;
    }

    constexpr VkDevice
    Device::get() const noexcept
    {
        return device_;
    }

    constexpr const std::vector<QueueFamily>&
    Device::getQueueFamilies() const noexcept
    {
        return queueFamilies_;
    }

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

    bool
    isPhysicalDeviceFeaturesSupported(const VkPhysicalDeviceFeatures* available,
                                      const VkPhysicalDeviceFeatures* required) noexcept;

    bool
    isPhysicalDeviceExtensionsSupported(const VkPhysicalDevice          physicalDevice,
                                        const std::vector<std::string>& extensions) noexcept;

    bool
    isPhysicalDeviceQueuesSupported(const VkPhysicalDevice physicalDevice, const VkQueueFlags queueFlags) noexcept;

    PhysicalDevice
    getPhysicalDevice(const Device::Configuration& configuration, const VkInstance instance);

    Device
    getDevice(const Device::Configuration& configuration, const VkInstance instance);
} // namespace nd::src::graphics::vulkan
