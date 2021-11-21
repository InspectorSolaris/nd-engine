#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct PhysicalDevice final
    {
        using Extension     = std::string;
        using Extensions    = std::vector<Extension>;
        using QueueFamily   = VkQueueFamilyProperties;
        using QueueFamilies = std::vector<QueueFamily>;

        VkPhysicalDevice           handle;
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures   features;
    };

    class Device final
    {
    public:
        struct QueueFamily final
        {
            const uint32_t     index;
            const uint32_t     queueCount;
            const VkQueueFlags queueFlags;
        };

        using Extension       = std::string;
        using Extensions      = std::vector<Extension>;
        using QueueFamilies   = std::vector<QueueFamily>;
        using QueueCreateInfo = VkDeviceQueueCreateInfo;
        using CreateInfo      = VkDeviceCreateInfo;

        struct Configuration final
        {
            const VkPhysicalDeviceFeatures& features;

            const std::function<size_t(const PhysicalDevice&)>& physicalDevicePriority;

            const Extensions& extensions;

            const VkQueueFlags queueFlags;
        };

        Device() noexcept;
        Device(const VkPhysicalDevice device, const QueueFamilies& queueFamilies, const CreateInfo& createInfo);

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

        constexpr const QueueFamilies&
        getQueueFamilies() const noexcept;

    private:
        QueueFamilies queueFamilies_ {};

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

    constexpr const Device::QueueFamilies&
    Device::getQueueFamilies() const noexcept
    {
        return queueFamilies_;
    }

    Device::QueueCreateInfo
    getQueueCreateInfo(const uint32_t queueFamilyIndex, const uint32_t queueCount, const float* queuePriorities) noexcept;

    Device::CreateInfo
    getDeviceCreateInfo(const uint32_t                  queueCreateInfosCount,
                        const uint32_t                  enabledExtensionsCount,
                        const VkDeviceQueueCreateInfo*  queueCreateInfos,
                        const char* const*              enabledExtensions,
                        const VkPhysicalDeviceFeatures* enabledFeatures) noexcept;

    PhysicalDevice::QueueFamilies
    getPhysicalDeviceQueueFamilies(const VkPhysicalDevice physicalDevice) noexcept;

    Device::QueueFamilies
    getDeviceQueueFamilies(const VkPhysicalDevice physicalDevice, const VkQueueFlags queueFlags) noexcept;

    bool
    isPhysicalDeviceExtensionsSupported(const VkPhysicalDevice            physicalDevice,
                                        const PhysicalDevice::Extensions& extensions) noexcept;

    bool
    isPhysicalDeviceFeaturesSupported(const VkPhysicalDeviceFeatures* available,
                                      const VkPhysicalDeviceFeatures* required) noexcept;

    bool
    isPhysicalDeviceQueuesSupported(const VkPhysicalDevice physicalDevice, const VkQueueFlags queueFlags) noexcept;

    PhysicalDevice
    getPhysicalDevice(const Device::Configuration& configuration, const VkInstance instance);

    Device
    getDevice(const Device::Configuration& configuration, const VkInstance instance);
} // namespace nd::src::graphics::vulkan
