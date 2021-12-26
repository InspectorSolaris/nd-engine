#pragma once

#include "pch.hpp"
#include "shared.hpp"

#include "instance.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "descriptor_pool.hpp"
#include "descriptor_set_layout.hpp"
#include "descriptor_set.hpp"
#include "render_pass.hpp"
#include "shader_module.hpp"
#include "pipeline_layout.hpp"
#include "pipeline.hpp"
#include "image.hpp"
#include "image_view.hpp"
#include "framebuffer.hpp"
#include "buffer.hpp"
#include "buffer_view.hpp"
#include "command_pool.hpp"
#include "command_buffer.hpp"
#include "synchronization.hpp"

namespace nd::src::graphics::vulkan
{
    struct VulkanContextConfigurationExternal final
    {
        const std::string& applicationName;
        const std::string& engineName;

        const std::vector<std::string>& layers;
        const std::vector<std::string>& extensions;

        const uint32_t width;
        const uint32_t height;
    };

    struct VulkanContextInitializers final
    {
        using Instance = VkInstance(const InstanceConfiguration&, const VkInstanceCreateFlags, const void*);

        using PhysicalDevice = VkPhysicalDevice(const PhysicalDeviceConfiguration&, const VkInstance);

        using Device = Device(const DeviceConfiguration&, const VkPhysicalDevice, const VkDeviceCreateFlags, const void*);

        using Surface = VkSurfaceKHR(const VkInstance);

        const std::function<Instance>       getInstance;
        const std::function<PhysicalDevice> getPhysicalDevice;
        const std::function<Device>         getDevice;
        const std::function<Surface>        getSurface;
    };

    struct VulkanContextConfigurations final
    {
        using Instance       = InstanceConfiguration(const VulkanContextConfigurationExternal&);
        using PhysicalDevice = PhysicalDeviceConfiguration();
        using Device         = DeviceConfiguration(const PhysicalDeviceConfiguration&);

        const std::function<Instance>       getInstanceConfiguration;
        const std::function<PhysicalDevice> getPhysicalDeviceConfiguration;
        const std::function<Device>         getDeviceConfiguration;
    };

    struct VulkanContextFilters final
    {
        using QueueFamily = bool(const QueueFamily, const size_t);

        const std::function<QueueFamily> deviceQueueFamilyFilter;
        const std::function<QueueFamily> swapchainQueueFamilyFilter;
    };

    InstanceConfiguration
    getInstanceConfiguration(const VulkanContextConfigurationExternal& configurationExternal) noexcept;

    PhysicalDeviceConfiguration
    getPhysicalDeviceConfiguration() noexcept;

    extern VulkanContextInitializers   vulkanContextInitializers;
    extern VulkanContextConfigurations vulkanContextConfigurations;
    extern VulkanContextFilters        vulkanContextFilters;
} // namespace nd::src::graphics::vulkan
