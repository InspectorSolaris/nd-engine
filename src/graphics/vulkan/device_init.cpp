#include "device_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    vec<VkPhysicalDevice>
    getPhysicalDevices(const VkInstance instance) noexcept(ND_VK_ASSERT_EXEC_NOTHROW)
    {
        ND_SET_SCOPE();

        u32 count;

        ND_VK_ASSERT_EXEC(vkEnumeratePhysicalDevices(instance, &count, nullptr));

        auto physicalDevices = vec<VkPhysicalDevice>(count);

        ND_VK_ASSERT_EXEC(vkEnumeratePhysicalDevices(instance, &count, physicalDevices.data()));

        return physicalDevices;
    }

    vec<VkExtensionProperties>
    getPhysicalDeviceExtensionProperties(const VkPhysicalDevice physicalDevice) noexcept(ND_VK_ASSERT_EXEC_NOTHROW)
    {
        ND_SET_SCOPE();

        u32 count;

        ND_VK_ASSERT_EXEC(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, nullptr));

        auto properties = vec<VkExtensionProperties>(count);

        ND_VK_ASSERT_EXEC(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, properties.data()));

        return properties;
    }

    vec<VkQueueFamilyProperties>
    getPhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice physicalDevice) noexcept
    {
        ND_SET_SCOPE();

        u32 count;

        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);

        auto properties = vec<VkQueueFamilyProperties>(count);

        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, properties.data());

        return properties;
    }

    vec<QueueFamily>
    getPhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice physicalDevice, const VkQueueFlags queueFlags) noexcept
    {
        ND_SET_SCOPE();

        const auto queueFamilyProperties = getPhysicalDeviceQueueFamilyProperties(physicalDevice);
        const auto queueFamilies         = getMapped<VkQueueFamilyProperties, QueueFamily>(
            queueFamilyProperties,
            [](const auto& properties, const auto index)
            {
                return QueueFamily {properties.queueFlags, static_cast<u16>(properties.queueCount), static_cast<u16>(index)};
            });

        return getFiltered<QueueFamily>(queueFamilies,
                                        [queueFlags](const auto& queueFamily, const auto index)
                                        {
                                            return isContainsAny(queueFamily.queueFlags, queueFlags);
                                        });
    }

    VkPhysicalDeviceFeatures
    getPhysicalDeviceFeatures(const VkPhysicalDevice physicalDevice) noexcept
    {
        ND_SET_SCOPE();

        VkPhysicalDeviceFeatures features;

        vkGetPhysicalDeviceFeatures(physicalDevice, &features);

        return features;
    }

    VkPhysicalDeviceProperties
    getPhysicalDeviceProperties(const VkPhysicalDevice physicalDevice) noexcept
    {
        ND_SET_SCOPE();

        VkPhysicalDeviceProperties properties;

        vkGetPhysicalDeviceProperties(physicalDevice, &properties);

        return properties;
    }

    bool
    isPhysicalDeviceExtensionsSupported(const VkPhysicalDevice physicalDevice, const vec<str>& extensions) noexcept(ND_VK_ASSERT_EXEC_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto physicalDeviceExtensions = getPhysicalDeviceExtensionProperties(physicalDevice);

        return std::all_of(extensions.begin(),
                           extensions.end(),
                           [&physicalDeviceExtensions](const auto& extension)
                           {
                               return std::any_of(physicalDeviceExtensions.begin(),
                                                  physicalDeviceExtensions.end(),
                                                  [&extension](const auto& physicalDeviceExtension)
                                                  {
                                                      return !std::strcmp(extension.c_str(), physicalDeviceExtension.extensionName);
                                                  });
                           });
    }

    bool
    isPhysicalDeviceFeaturesSupported(const VkPhysicalDevice physicalDevice, const VkPhysicalDeviceFeatures& features) noexcept
    {
        ND_SET_SCOPE();

        const auto physicalDeviceFeatures = getPhysicalDeviceFeatures(physicalDevice);

        auto shift = 0ULL;

        const auto step = sizeof(VkBool32);
        const auto size = sizeof(VkPhysicalDeviceFeatures);

        const auto featuresPtr               = &features;
        const auto physicalDeviceFeaturesPtr = &physicalDeviceFeatures;

        while(shift * step < size)
        {
            const auto feature               = *(reinterpret_cast<const VkBool32*>(featuresPtr) + shift);
            const auto physicalDeviceFeature = *(reinterpret_cast<const VkBool32*>(physicalDeviceFeaturesPtr) + shift);

            if(feature && !physicalDeviceFeature)
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
        ND_SET_SCOPE();

        const auto physicalDeviceQueueFamilies = getPhysicalDeviceQueueFamilyProperties(physicalDevice);

        auto queueFlagsSupported = VkQueueFlags {};

        for(const auto& queueFamily: physicalDeviceQueueFamilies)
        {
            queueFlagsSupported |= queueFamily.queueFlags;
        }

        return isContainsAll(queueFlagsSupported, queueFlags);
    }

    PhysicalDevice
    getPhysicalDevice(opt<const PhysicalDeviceCfg>::ref cfg, const VkInstance instance) noexcept(ND_VK_ASSERT_EXEC_NOTHROW&& ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto physicalDevices = getPhysicalDevices(instance);

        auto physicalDeviceMax         = std::optional<VkPhysicalDevice> {};
        auto physicalDevicePriorityMax = std::optional<u64> {};

        for(const auto physicalDevice: physicalDevices)
        {
            const auto features   = getPhysicalDeviceFeatures(physicalDevice);
            const auto properties = getPhysicalDeviceProperties(physicalDevice);
            const auto priority   = cfg.priority(features, properties);

            if(isPhysicalDeviceExtensionsSupported(physicalDevice, cfg.extensions) &&
               isPhysicalDeviceFeaturesSupported(physicalDevice, cfg.features) && isPhysicalDeviceQueuesSupported(physicalDevice, cfg.queueFlags) &&
               physicalDevicePriorityMax < priority)
            {
                physicalDeviceMax         = physicalDevice;
                physicalDevicePriorityMax = priority;
            }
        }

        ND_ASSERT(physicalDeviceMax.has_value());

        return physicalDeviceMax.value();
    }

    Device
    createDevice(opt<const DeviceCfg>::ref cfg, const VkPhysicalDevice physicalDevice) noexcept(ND_VK_ASSERT_EXEC_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto cextensions   = getRawStrings(cfg.extensions);
        const auto queueFamilies = getPhysicalDeviceQueueFamilyProperties(physicalDevice, cfg.queueFlags);

        const auto cextensionsSize   = static_cast<u32>(cextensions.size());
        const auto queueFamiliesSize = static_cast<u32>(queueFamilies.size());

        auto queuePriorities  = vec<vec<f32>> {};
        auto queueCreateInfos = vec<VkDeviceQueueCreateInfo> {};

        queuePriorities.reserve(queueFamilies.size());
        queueCreateInfos.reserve(queueFamilies.size());

        for(u16 index = 0; index < queueFamilies.size(); ++index)
        {
            const auto queueFamily = queueFamilies[index];

            queuePriorities.push_back(vec<f32>(queueFamily.queueCount, 1.0f));
            queueCreateInfos.push_back(VkDeviceQueueCreateInfo {.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                                                .pNext            = {},
                                                                .flags            = {},
                                                                .queueFamilyIndex = static_cast<u32>(queueFamily.index),
                                                                .queueCount       = queueFamily.queueCount,
                                                                .pQueuePriorities = queuePriorities[index].data()});
        }

        const auto createInfo = VkDeviceCreateInfo {.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                                                    .pNext                   = cfg.next,
                                                    .flags                   = cfg.flags,
                                                    .queueCreateInfoCount    = queueFamiliesSize,
                                                    .pQueueCreateInfos       = queueCreateInfos.data(),
                                                    .enabledLayerCount       = 0U,
                                                    .ppEnabledLayerNames     = nullptr,
                                                    .enabledExtensionCount   = cextensionsSize,
                                                    .ppEnabledExtensionNames = cextensions.data(),
                                                    .pEnabledFeatures        = &cfg.features};

        VkDevice device;

        ND_VK_ASSERT_EXEC(vkCreateDevice(physicalDevice, &createInfo, ND_VK_ALLOCATION_CALLBACKS, &device));

        const auto memoryProperties = getMemoryProperties(physicalDevice);

        return {.memory      = {.device = allocateMemory(cfg.memory.device, memoryProperties, device),
                                .host   = allocateMemory(cfg.memory.host, memoryProperties, device)},
                .queueFamily = {.graphics = getQueueFamily(cfg.queueFamily.graphics, queueFamilies),
                                .transfer = getQueueFamily(cfg.queueFamily.transfer, queueFamilies),
                                .compute  = getQueueFamily(cfg.queueFamily.compute, queueFamilies)},
                .handle      = device};
    }
} // namespace nd::src::graphics::vulkan
