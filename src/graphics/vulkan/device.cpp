#include "device.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VkDeviceQueueCreateInfo
    getQueueCreateInfo(const uint32_t                 queueFamilyIndex,
                       const uint32_t                 queueCount,
                       const float*                   queuePriorities,
                       const VkDeviceQueueCreateFlags flags,
                       const void*                    next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, // sType;
            next,                                       // pNext;
            flags,                                      // flags;
            queueFamilyIndex,                           // queueFamilyIndex;
            queueCount,                                 // queueCount;
            queuePriorities                             // pQueuePriorities;
        };
    }

    VkDeviceCreateInfo
    getDeviceCreateInfo(const uint32_t                  queueCreateInfosCount,
                        const uint32_t                  enabledExtensionsCount,
                        const VkDeviceQueueCreateInfo*  queueCreateInfos,
                        const char* const*              enabledExtensions,
                        const VkPhysicalDeviceFeatures* enabledFeatures,
                        const VkDeviceCreateFlags       flags,
                        const void*                     next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, // sType;
            next,                                 // pNext;
            flags,                                // flags;
            queueCreateInfosCount,                // queueCreateInfoCount;
            queueCreateInfos,                     // pQueueCreateInfos;
            0ULL,                                 // enabledLayerCount;
            nullptr,                              // ppEnabledLayerNames;
            enabledExtensionsCount,               // enabledExtensionCount;
            enabledExtensions,                    // ppEnabledExtensionNames;
            enabledFeatures                       // pEnabledFeatures;
        };
    }

    std::vector<VkPhysicalDevice>
    getPhysicalDevices(const VkInstance instance) noexcept
    {
        ND_SET_SCOPE();

        uint32_t count;

        vkEnumeratePhysicalDevices(instance, &count, nullptr);

        auto physicalDevices = std::vector<VkPhysicalDevice>(count);

        vkEnumeratePhysicalDevices(instance, &count, physicalDevices.data());

        return physicalDevices;
    }

    bool
    isPhysicalDeviceFeaturesSupported(const VkPhysicalDeviceFeatures* available,
                                      const VkPhysicalDeviceFeatures* required) noexcept
    {
        ND_SET_SCOPE();

        auto shift = 0ULL;

        const auto step = sizeof(VkBool32);
        const auto size = sizeof(VkPhysicalDeviceFeatures);

        while(shift * step < size)
        {
            const auto availableValue = reinterpret_cast<const VkBool32*>(available) + shift;
            const auto requiredValue  = reinterpret_cast<const VkBool32*>(required) + shift;

            if(*requiredValue && !*availableValue)
            {
                return false;
            }

            ++shift;
        }

        return true;
    }

    bool
    isPhysicalDeviceExtensionsSupported(const VkPhysicalDevice          physicalDevice,
                                        const std::vector<std::string>& extensions) noexcept
    {
        ND_SET_SCOPE();

        uint32_t count;

        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, nullptr);

        auto extensionsProperties = std::vector<VkExtensionProperties>(count);

        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, extensionsProperties.data());

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

    bool
    isPhysicalDeviceQueuesSupported(const VkPhysicalDevice physicalDevice, const VkQueueFlags queueFlags) noexcept
    {
        ND_SET_SCOPE();

        auto queueFlagsPool      = queueFlags;
        auto queueFlagsSupported = VkQueueFlags {};

        const auto queueFamilies = getQueueFamiliesProperties(physicalDevice);

        while(queueFlagsPool)
        {
            const auto queueFlag = static_cast<VkQueueFlagBits>(getNextBit(queueFlagsPool));

            if(std::any_of(queueFamilies.begin(),
                           queueFamilies.end(),
                           [queueFlag](const auto& queueFamily)
                           {
                               return isSubmask(queueFamily.queueFlags, queueFlag);
                           }))
            {
                queueFlagsSupported |= queueFlag;
            }
            else
            {
                return false;
            }

            queueFlagsPool -= queueFlag;
        }

        return queueFlags == queueFlagsSupported;
    }

    VkPhysicalDevice
    getPhysicalDevice(const PhysicalDeviceConfiguration& configuration, const VkInstance instance)
    {
        ND_SET_SCOPE();

        const auto physicalDevices = getPhysicalDevices(instance);

        auto physicalDeviceMax         = std::optional<VkPhysicalDevice> {};
        auto physicalDevicePriorityMax = std::optional<size_t> {};

        for(const auto& physicalDevice: physicalDevices)
        {
            auto properties = VkPhysicalDeviceProperties {};
            auto features   = VkPhysicalDeviceFeatures {};

            vkGetPhysicalDeviceProperties(physicalDevice, &properties);
            vkGetPhysicalDeviceFeatures(physicalDevice, &features);

            const auto priority = configuration.priority(physicalDevice, properties, features);

            if(isPhysicalDeviceFeaturesSupported(&features, &configuration.features) &&
               isPhysicalDeviceExtensionsSupported(physicalDevice, configuration.extensions) &&
               isPhysicalDeviceQueuesSupported(physicalDevice, configuration.queueFlags) &&
               (!physicalDevicePriorityMax.has_value() || physicalDevicePriorityMax < priority))
            {
                physicalDeviceMax         = physicalDevice;
                physicalDevicePriorityMax = priority;
            }
        }

        ND_ASSERT(physicalDeviceMax.has_value());

        return physicalDeviceMax.value();
    }

    VkDevice
    getDeviceHandle(const VkDeviceCreateInfo& createInfo, const VkPhysicalDevice physicalDevice)
    {
        ND_SET_SCOPE();

        VkDevice device;

        ND_ASSERT(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) == VK_SUCCESS);

        return device;
    }

    Device
    getDevice(const DeviceConfiguration& configuration,
              const VkPhysicalDevice     physicalDevice,
              const VkDeviceCreateFlags  flags,
              const void*                next)
    {
        ND_SET_SCOPE();

        const auto cextensions = getRawStrings(configuration.extensions);

        auto queueFamilies    = getQueueFamilies(physicalDevice);
        auto queuePriorities  = std::vector<std::vector<float>> {};
        auto queueCreateInfos = std::vector<VkDeviceQueueCreateInfo> {};

        for(const auto& queueFamily: queueFamilies)
        {
            queueCreateInfos.push_back(
                getQueueCreateInfo(queueFamily.index,
                                   queueFamily.queueCount,
                                   queuePriorities.emplace_back(queueFamily.queueCount, 1.0f).data()));
        }

        const auto createInfo = getDeviceCreateInfo(queueCreateInfos.size(),
                                                    cextensions.size(),
                                                    queueCreateInfos.data(),
                                                    cextensions.data(),
                                                    &configuration.features,
                                                    flags,
                                                    next);

        return {std::move(queueFamilies), getDeviceHandle(createInfo, physicalDevice)};
    }
} // namespace nd::src::graphics::vulkan
