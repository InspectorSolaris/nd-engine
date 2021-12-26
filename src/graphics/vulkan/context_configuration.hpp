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

    class VulkanContextInitializersBuilder final
    {
    public:
        VulkanContextInitializers
        build() const noexcept
        {
            return {getInstance,
                    getPhysicalDevice,
                    getDevice,
                    getSurface,
                    getSwapchain,
                    getRenderPass,
                    getSwapchainImageView,
                    getSwapchainFramebuffer};
        }

        operator VulkanContextInitializers() const noexcept
        {
            return build();
        }

        VulkanContextInitializersBuilder&
        add(decltype(VulkanContextInitializers::getInstance)& initializer) noexcept
        {
            getInstance = initializer;

            return *this;
        }

        VulkanContextInitializersBuilder&
        add(decltype(VulkanContextInitializers::getPhysicalDevice)& initializer) noexcept
        {
            getPhysicalDevice = initializer;

            return *this;
        }

        VulkanContextInitializersBuilder&
        add(decltype(VulkanContextInitializers::getDevice)& initializer) noexcept
        {
            getDevice = initializer;

            return *this;
        }

        VulkanContextInitializersBuilder&
        add(decltype(VulkanContextInitializers::getSurface)& initializer) noexcept
        {
            getSurface = initializer;

            return *this;
        }

        VulkanContextInitializersBuilder&
        add(decltype(VulkanContextInitializers::getSwapchain)& initializer) noexcept
        {
            getSwapchain = initializer;

            return *this;
        }

        VulkanContextInitializersBuilder&
        add(decltype(VulkanContextInitializers::getRenderPass)& initializer) noexcept
        {
            getRenderPass = initializer;

            return *this;
        }

        VulkanContextInitializersBuilder&
        add(decltype(VulkanContextInitializers::getSwapchainImageView)& initializer) noexcept
        {
            getSwapchainImageView = initializer;

            return *this;
        }

        VulkanContextInitializersBuilder&
        add(decltype(VulkanContextInitializers::getSwapchainFramebuffer)& initializer) noexcept
        {
            getSwapchainFramebuffer = initializer;

            return *this;
        }

        VulkanContextInitializersBuilder&
        operator<<(decltype(VulkanContextInitializers::getInstance)& initializer) noexcept
        {
            return add(initializer);
        }

        VulkanContextInitializersBuilder&
        operator<<(decltype(VulkanContextInitializers::getPhysicalDevice)& initializer) noexcept
        {
            return add(initializer);
        }

        VulkanContextInitializersBuilder&
        operator<<(decltype(VulkanContextInitializers::getDevice)& initializer) noexcept
        {
            return add(initializer);
        }

        VulkanContextInitializersBuilder&
        operator<<(decltype(VulkanContextInitializers::getSurface)& initializer) noexcept
        {
            return add(initializer);
        }

        VulkanContextInitializersBuilder&
        operator<<(decltype(VulkanContextInitializers::getSwapchain)& initializer) noexcept
        {
            return add(initializer);
        }

        VulkanContextInitializersBuilder&
        operator<<(decltype(VulkanContextInitializers::getRenderPass)& initializer) noexcept
        {
            return add(initializer);
        }

        VulkanContextInitializersBuilder&
        operator<<(decltype(VulkanContextInitializers::getSwapchainImageView)& initializer) noexcept
        {
            return add(initializer);
        }

        VulkanContextInitializersBuilder&
        operator<<(decltype(VulkanContextInitializers::getSwapchainFramebuffer)& initializer) noexcept
        {
            return add(initializer);
        }

    private:
        std::remove_cv_t<decltype(VulkanContextInitializers::getInstance)>             getInstance;
        std::remove_cv_t<decltype(VulkanContextInitializers::getPhysicalDevice)>       getPhysicalDevice;
        std::remove_cv_t<decltype(VulkanContextInitializers::getDevice)>               getDevice;
        std::remove_cv_t<decltype(VulkanContextInitializers::getSurface)>              getSurface;
        std::remove_cv_t<decltype(VulkanContextInitializers::getSwapchain)>            getSwapchain;
        std::remove_cv_t<decltype(VulkanContextInitializers::getRenderPass)>           getRenderPass;
        std::remove_cv_t<decltype(VulkanContextInitializers::getSwapchainImageView)>   getSwapchainImageView;
        std::remove_cv_t<decltype(VulkanContextInitializers::getSwapchainFramebuffer)> getSwapchainFramebuffer;
    };

    class VulkanContextConfigurationsBuilder final
    {
    public:
        VulkanContextConfigurations
        build() const noexcept
        {
            return {getInstance,
                    getPhysicalDevice,
                    getDevice,
                    getSwapchain,
                    getRenderPass,
                    getSwapchainImageView,
                    getSwapchainFramebuffer};
        }

        operator VulkanContextConfigurations() const noexcept
        {
            return build();
        }

        VulkanContextConfigurationsBuilder&
        add(decltype(VulkanContextConfigurations::getInstanceConfiguration)& configuration) noexcept
        {
            getInstance = configuration;

            return *this;
        }

        VulkanContextConfigurationsBuilder&
        add(decltype(VulkanContextConfigurations::getPhysicalDeviceConfiguration)& configuration) noexcept
        {
            getPhysicalDevice = configuration;

            return *this;
        }

        VulkanContextConfigurationsBuilder&
        add(decltype(VulkanContextConfigurations::getDeviceConfiguration)& configuration) noexcept
        {
            getDevice = configuration;

            return *this;
        }

        VulkanContextConfigurationsBuilder&
        add(decltype(VulkanContextConfigurations::getSwapchainConfiguration)& configuration) noexcept
        {
            getSwapchain = configuration;

            return *this;
        }

        VulkanContextConfigurationsBuilder&
        add(decltype(VulkanContextConfigurations::getRenderPassConfiguration)& configuration) noexcept
        {
            getRenderPass = configuration;

            return *this;
        }

        VulkanContextConfigurationsBuilder&
        add(decltype(VulkanContextConfigurations::getSwapchainImageViewConfiguration)& configuration) noexcept
        {
            getSwapchainImageView = configuration;

            return *this;
        }

        VulkanContextConfigurationsBuilder&
        add(decltype(VulkanContextConfigurations::getSwapchainFramebufferConfiguration)& configuration) noexcept
        {
            getSwapchainFramebuffer = configuration;

            return *this;
        }

        VulkanContextConfigurationsBuilder&
        operator<<(decltype(VulkanContextConfigurations::getInstanceConfiguration)& configuration) noexcept
        {
            return add(configuration);
        }

        VulkanContextConfigurationsBuilder&
        operator<<(decltype(VulkanContextConfigurations::getPhysicalDeviceConfiguration)& configuration) noexcept
        {
            return add(configuration);
        }

        VulkanContextConfigurationsBuilder&
        operator<<(decltype(VulkanContextConfigurations::getDeviceConfiguration)& configuration) noexcept
        {
            return add(configuration);
        }

        VulkanContextConfigurationsBuilder&
        operator<<(decltype(VulkanContextConfigurations::getSwapchainConfiguration)& configuration) noexcept
        {
            return add(configuration);
        }

        VulkanContextConfigurationsBuilder&
        operator<<(decltype(VulkanContextConfigurations::getRenderPassConfiguration)& configuration) noexcept
        {
            return add(configuration);
        }

        VulkanContextConfigurationsBuilder&
        operator<<(decltype(VulkanContextConfigurations::getSwapchainImageViewConfiguration)& configuration) noexcept
        {
            return add(configuration);
        }

        VulkanContextConfigurationsBuilder&
        operator<<(decltype(VulkanContextConfigurations::getSwapchainFramebufferConfiguration)& configuration) noexcept
        {
            return add(configuration);
        }

    private:
        std::remove_cv_t<decltype(VulkanContextConfigurations::getInstanceConfiguration)>           getInstance;
        std::remove_cv_t<decltype(VulkanContextConfigurations::getPhysicalDeviceConfiguration)>     getPhysicalDevice;
        std::remove_cv_t<decltype(VulkanContextConfigurations::getDeviceConfiguration)>             getDevice;
        std::remove_cv_t<decltype(VulkanContextConfigurations::getSwapchainConfiguration)>          getSwapchain;
        std::remove_cv_t<decltype(VulkanContextConfigurations::getRenderPassConfiguration)>         getRenderPass;
        std::remove_cv_t<decltype(VulkanContextConfigurations::getSwapchainImageViewConfiguration)> getSwapchainImageView;
        std::remove_cv_t<decltype(VulkanContextConfigurations::getSwapchainFramebufferConfiguration)>
            getSwapchainFramebuffer;
    };

    extern VulkanContextInitializersBuilder   initializersBuilder;
    extern VulkanContextConfigurationsBuilder configurationsBuilder;
} // namespace nd::src::graphics::vulkan
