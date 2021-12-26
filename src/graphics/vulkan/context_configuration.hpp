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
        using Instance = VkInstance(const InstanceConfiguration&);

        using PhysicalDevice = VkPhysicalDevice(const PhysicalDeviceConfiguration&, const VkInstance);

        using Device = Device(const DeviceConfiguration&, const VkPhysicalDevice);

        using Surface = VkSurfaceKHR(const VkInstance);

        using Swapchain = Swapchain(const SwapchainConfiguration&, const VkDevice);

        using RenderPass = VkRenderPass(const RenderPassConfiguration&, const VkDevice);

        using ImageView = VkImageView(const ImageViewConfiguration&, const VkDevice);

        using Framebuffer = VkFramebuffer(const FramebufferConfiguration&, const VkDevice);

        const std::function<Instance>       getInstance;
        const std::function<PhysicalDevice> getPhysicalDevice;
        const std::function<Device>         getDevice;
        const std::function<Surface>        getSurface;
        const std::function<Swapchain>      getSwapchain;
        const std::function<RenderPass>     getRenderPass;
        const std::function<ImageView>      getSwapchainImageView;
        const std::function<Framebuffer>    getSwapchainFramebuffer;
    };

    struct VulkanContextConfigurations final
    {
        using Instance = InstanceConfiguration(const VulkanContextConfigurationExternal&);

        using PhysicalDevice = PhysicalDeviceConfiguration();

        using Device = DeviceConfiguration(const PhysicalDeviceConfiguration&);

        using Swapchain = SwapchainConfiguration(const VkPhysicalDevice physicalDevice,
                                                 const VkSurfaceKHR     surface,
                                                 const uint32_t         width,
                                                 const uint32_t         height);

        using RenderPass = RenderPassConfiguration(const SwapchainConfiguration&);

        using SwapchainImageView = ImageViewConfiguration(const SwapchainConfiguration&, const VkImage image);

        using SwapchainFramebuffer = FramebufferConfiguration(const SwapchainConfiguration&,
                                                              const VkImageView,
                                                              const VkRenderPass);

        const std::function<Instance>             getInstanceConfiguration;
        const std::function<PhysicalDevice>       getPhysicalDeviceConfiguration;
        const std::function<Device>               getDeviceConfiguration;
        const std::function<Swapchain>            getSwapchainConfiguration;
        const std::function<RenderPass>           getRenderPassConfiguration;
        const std::function<SwapchainImageView>   getSwapchainImageViewConfiguration;
        const std::function<SwapchainFramebuffer> getSwapchainFramebufferConfiguration;
    };

    InstanceConfiguration
    getInstanceConfiguration(const VulkanContextConfigurationExternal& configurationExternal) noexcept;

    PhysicalDeviceConfiguration
    getPhysicalDeviceConfiguration() noexcept;

    DeviceConfiguration
    getDeviceConfiguration(const PhysicalDeviceConfiguration& physicalDeviceConfiguration) noexcept;

    SwapchainConfiguration
    getSwapchainConfiguration(const VkPhysicalDevice physicalDevice,
                              const VkSurfaceKHR     surface,
                              const uint32_t         width,
                              const uint32_t         height) noexcept;

    RenderPassConfiguration
    getRenderPassConfiguration(const SwapchainConfiguration& swapchainConfiguration) noexcept;

    ImageViewConfiguration
    getSwapchainImageViewConfiguration(const SwapchainConfiguration& swapchainConfiguration, const VkImage image) noexcept;

    FramebufferConfiguration
    getSwapchainFramebufferConfiguration(const SwapchainConfiguration& swapchainConfiguration,
                                         const VkImageView             imageView,
                                         const VkRenderPass            renderPass) noexcept;

    extern VulkanContextInitializers   vulkanContextInitializers;
    extern VulkanContextConfigurations vulkanContextConfigurations;
} // namespace nd::src::graphics::vulkan
