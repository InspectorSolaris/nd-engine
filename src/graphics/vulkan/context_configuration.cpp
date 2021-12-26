#include "context_configuration.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VulkanContextInitializers   vulkanContextInitializers   = {getInstance, getPhysicalDevice, getDevice};
    VulkanContextConfigurations vulkanContextConfigurations = {getInstanceConfiguration,
                                                               getPhysicalDeviceConfiguration,
                                                               getDeviceConfiguration};

    InstanceConfiguration
    getInstanceConfiguration(const VulkanContextConfigurationExternal& configurationExternal) noexcept
    {
        ND_SET_SCOPE();

        const auto layers = getMerged(configurationExternal.layers,
                                      {
#ifndef NDEBUG
                                          "VK_LAYER_KHRONOS_validation"
#endif
                                      });

        const auto extensions = getMerged(configurationExternal.extensions, {});

        return {configurationExternal.applicationName,
                configurationExternal.engineName,
                std::move(layers),
                std::move(extensions),
                VK_MAKE_VERSION(0, 1, 0),
                VK_MAKE_VERSION(0, 1, 0),
                VK_API_VERSION_1_2};
    }

    PhysicalDeviceConfiguration
    getPhysicalDeviceConfiguration() noexcept
    {
        ND_SET_SCOPE();

        return {{},
                [](const VkPhysicalDevice            physicalDevice,
                   const VkPhysicalDeviceProperties& properties,
                   const VkPhysicalDeviceFeatures&   features)
                {
                    return 1;
                },
                {"VK_KHR_swapchain"},
                VK_QUEUE_GRAPHICS_BIT};
    }

    DeviceConfiguration
    getDeviceConfiguration(const PhysicalDeviceConfiguration& physicalDeviceConfiguration) noexcept
    {
        ND_SET_SCOPE();

        return {physicalDeviceConfiguration.features, physicalDeviceConfiguration.extensions};
    }
} // namespace nd::src::graphics::vulkan
