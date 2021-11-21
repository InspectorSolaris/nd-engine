#include "device.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    Device::Device() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    Device::Device(const VkPhysicalDevice physicalDevice, const QueueFamilies& queueFamilies, const CreateInfo& createInfo)
        : physicalDevice_(physicalDevice)
        , queueFamilies_(queueFamilies)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device_) == VK_SUCCESS);
    }

    Device::Device(Device&& device) noexcept
        : physicalDevice_(std::move(device.physicalDevice_))
        , device_(std::move(device.device_))
        , queueFamilies_(std::move(device.queueFamilies_))
    {
        ND_SET_SCOPE_LOW();

        device.physicalDevice_ = VK_NULL_HANDLE;
        device.device_         = VK_NULL_HANDLE;
        device.queueFamilies_  = QueueFamilies {};
    }

    Device&
    Device::operator=(Device&& device) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&device == this)
        {
            return *this;
        }

        physicalDevice_ = std::move(device.physicalDevice_);
        device_         = std::move(device.device_);
        queueFamilies_  = std::move(device.queueFamilies_);

        device.physicalDevice_ = VK_NULL_HANDLE;
        device.device_         = VK_NULL_HANDLE;
        device.queueFamilies_  = QueueFamilies {};

        return *this;
    }

    Device::~Device()
    {
        ND_SET_SCOPE_LOW();

        vkDestroyDevice(device_, nullptr);
    }

    Device::QueueCreateInfo
    getQueueCreateInfo(const uint32_t queueFamilyIndex, const uint32_t queueCount, const float* queuePriorities) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, // sType;
            nullptr,                                    // pNext;
            0,                                          // flags;
            queueFamilyIndex,                           // queueFamilyIndex;
            queueCount,                                 // queueCount;
            queuePriorities                             // pQueuePriorities;
        };
    }

    Device::CreateInfo
    getDeviceCreateInfo(const uint32_t                  queueCreateInfosCount,
                        const uint32_t                  enabledExtensionsCount,
                        const VkDeviceQueueCreateInfo*  queueCreateInfos,
                        const char* const*              enabledExtensions,
                        const VkPhysicalDeviceFeatures* enabledFeatures) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, // sType;
            nullptr,                              // pNext;
            0,                                    // flags;
            queueCreateInfosCount,                // queueCreateInfoCount;
            queueCreateInfos,                     // pQueueCreateInfos;
            0ULL,                                 // enabledLayerCount;
            nullptr,                              // ppEnabledLayerNames;
            enabledExtensionsCount,               // enabledExtensionCount;
            enabledExtensions,                    // ppEnabledExtensionNames;
            enabledFeatures                       // pEnabledFeatures;
        };
    }

    PhysicalDevice::QueueFamilies
    getPhysicalDeviceQueueFamilies(const VkPhysicalDevice physicalDevice) noexcept
    {
        ND_SET_SCOPE_LOW();

        uint32_t count;

        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);

        auto queueFamilies = PhysicalDevice::QueueFamilies(count);

        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, queueFamilies.data());

        return queueFamilies;
    }

    Device::QueueFamilies
    getDeviceQueueFamilies(const VkPhysicalDevice physicalDevice, const VkQueueFlags queueFlags) noexcept
    {
        ND_SET_SCOPE_LOW();

        const auto queueFamiliesRaw = getPhysicalDeviceQueueFamilies(physicalDevice);

        auto queueFamilies = Device::QueueFamilies {};

        for(size_t i = 0; i < queueFamiliesRaw.size(); ++i)
        {
            const auto queueFamilyRaw = queueFamiliesRaw[i];

            if(queueFamilyRaw.queueFlags & queueFlags)
            {
                queueFamilies.push_back(
                    Device::QueueFamily {static_cast<uint32_t>(i), queueFamilyRaw.queueCount, queueFamilyRaw.queueFlags});
            }
        }

        return queueFamilies;
    }

    bool
    isPhysicalDeviceExtensionsSupported(const VkPhysicalDevice            physicalDevice,
                                        const PhysicalDevice::Extensions& extensions) noexcept
    {
        ND_SET_SCOPE_LOW();

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
    isPhysicalDeviceFeaturesSupported(const VkPhysicalDeviceFeatures* available,
                                      const VkPhysicalDeviceFeatures* required) noexcept
    {
        ND_SET_SCOPE_LOW();

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
    isPhysicalDeviceQueuesSupported(const VkPhysicalDevice physicalDevice, const VkQueueFlags queueFlags) noexcept
    {
        ND_SET_SCOPE_LOW();

        auto queueFlagsLeft      = queueFlags;
        auto queueFlagsSupported = VkQueueFlags {};

        const auto queueFamilies = getPhysicalDeviceQueueFamilies(physicalDevice);

        while(queueFlagsLeft)
        {
            const auto queueFlag = static_cast<VkQueueFlagBits>(getNextBit(queueFlagsLeft));

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

            queueFlagsLeft -= queueFlag;
        }

        return queueFlags == queueFlagsSupported;
    }

    PhysicalDevice
    getPhysicalDevice(const Device::Configuration& configuration, const VkInstance instance)
    {
        ND_SET_SCOPE_LOW();

        uint32_t count;

        vkEnumeratePhysicalDevices(instance, &count, nullptr);

        auto physicalDevices = std::vector<VkPhysicalDevice>(count);

        vkEnumeratePhysicalDevices(instance, &count, physicalDevices.data());

        auto physicalDeviceMax         = std::optional<PhysicalDevice> {};
        auto physicalDevicePriorityMax = std::optional<size_t> {};

        for(const auto& physicalDevice: physicalDevices)
        {
            auto properties = VkPhysicalDeviceProperties {};
            auto features   = VkPhysicalDeviceFeatures {};

            vkGetPhysicalDeviceProperties(physicalDevice, &properties);
            vkGetPhysicalDeviceFeatures(physicalDevice, &features);

            auto priority = configuration.physicalDevicePriority({physicalDevice, properties, features});

            if(isPhysicalDeviceExtensionsSupported(physicalDevice, configuration.extensions) &&
               isPhysicalDeviceFeaturesSupported(&features, &configuration.features) &&
               isPhysicalDeviceQueuesSupported(physicalDevice, configuration.queueFlags) &&
               (!physicalDevicePriorityMax.has_value() || physicalDevicePriorityMax < priority))
            {
                physicalDeviceMax         = {physicalDevice, properties, features};
                physicalDevicePriorityMax = priority;
            }
        }

        ND_ASSERT(physicalDeviceMax.has_value());

        return physicalDeviceMax.value();
    }

    Device
    getDevice(const Device::Configuration& configuration, const VkInstance instance)
    {
        ND_SET_SCOPE_LOW();

        const auto physicalDevice = getPhysicalDevice(configuration, instance);

        const auto cextensions   = getCStrings(configuration.extensions);
        const auto queueFamilies = getDeviceQueueFamilies(physicalDevice.handle, configuration.queueFlags);

        auto queuePriorities  = std::vector<std::vector<float>> {};
        auto queueCreateInfos = std::vector<VkDeviceQueueCreateInfo> {};

        for(const auto& queueFamily: queueFamilies)
        {
            if(queueFamily.queueFlags & configuration.queueFlags)
            {
                queuePriorities.emplace_back(queueFamily.queueCount, 1.0f);
                queueCreateInfos.push_back(
                    getQueueCreateInfo(queueFamily.index, queueFamily.queueCount, queuePriorities.back().data()));
            }
        }

        const auto createInfo = getDeviceCreateInfo(queueCreateInfos.size(),
                                                    cextensions.size(),
                                                    queueCreateInfos.data(),
                                                    cextensions.data(),
                                                    &configuration.features);

        return Device(physicalDevice.handle, queueFamilies, createInfo);
    }
} // namespace nd::src::graphics::vulkan
