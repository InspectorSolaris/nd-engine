#include "context_configuration.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VulkanContextInitializers   vulkanContextInitializers   = {getInstance, getPhysicalDevice, getDevice};
    VulkanContextConfigurations vulkanContextConfigurations = {getInstanceConfiguration,
                                                               getPhysicalDeviceConfiguration,
                                                               getDeviceConfiguration,
                                                               getSwapchainConfiguration};

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

    SwapchainConfiguration
    getSwapchainConfiguration(const VkPhysicalDevice physicalDevice,
                              const VkSurfaceKHR     surface,
                              const uint32_t         width,
                              const uint32_t         height) noexcept
    {
        ND_SET_SCOPE();

        return {physicalDevice,
                surface,
                {width, height},
                1,
                1,
                true,
                VK_FORMAT_B8G8R8A8_SRGB,
                VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                VK_PRESENT_MODE_IMMEDIATE_KHR,
                VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR};
    }
} // namespace nd::src::graphics::vulkan
