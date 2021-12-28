#pragma once

#include "pch.hpp"

#include "shared.hpp"

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
        using Instance = Instance(const InstanceConfiguration&);

        using PhysicalDevice = PhysicalDevice(const PhysicalDeviceConfiguration&, const VkInstance);

        using Device = Device(const DeviceConfiguration&, const VkPhysicalDevice);

        using Surface = Surface(const VkInstance);

        using Swapchain = Swapchain(const SwapchainConfiguration&, const VkDevice);

        using RenderPass = RenderPass(const RenderPassConfiguration&, const VkDevice);

        using SwapchainImages = std::vector<Image>(const VkSwapchainKHR, const VkDevice);

        using SwapchainImageViews = std::vector<ImageView>(const std::vector<ImageViewConfiguration>&, const VkDevice);

        using SwapchainFramebuffers = std::vector<Framebuffer>(const std::vector<FramebufferConfiguration>&, const VkDevice);

        using ShaderModules = std::vector<ShaderModule>(const std::vector<ShaderModuleConfiguration>&, const VkDevice);

        using DescriptorPool = DescriptorPool(const DescriptorPoolConfiguration&, const VkDevice);

        using DescriptorSetLayout = DescriptorSetLayout(const DescriptorSetLayoutConfiguration&, const VkDevice);

        using DescriptorSets = DescriptorSets(const DescriptorSetsConfiguration&, const VkDevice);

        using PipelineLayout = PipelineLayout(const PipelineLayoutConfiguration&, const VkDevice);

        const std::function<Instance>              getInstance;
        const std::function<PhysicalDevice>        getPhysicalDevice;
        const std::function<Device>                getDevice;
        const std::function<Surface>               getSurface;
        const std::function<Swapchain>             getSwapchain;
        const std::function<RenderPass>            getRenderPass;
        const std::function<SwapchainImages>       getSwapchainImages;
        const std::function<SwapchainImageViews>   getSwapchainImageViews;
        const std::function<SwapchainFramebuffers> getSwapchainFramebuffers;
        const std::function<ShaderModules>         getShaderModules;
        const std::function<DescriptorPool>        getDescriptorPool;
        const std::function<DescriptorSetLayout>   getDescriptorSetLayout;
        const std::function<DescriptorSets>        getDescriptorSets;
        const std::function<PipelineLayout>        getPipelineLayout;
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

        using SwapchainImageViews = std::vector<ImageViewConfiguration>(const SwapchainConfiguration&, const std::vector<Image>&);

        using SwapchainFramebuffers = std::vector<FramebufferConfiguration>(const SwapchainConfiguration&,
                                                                            const std::vector<ImageView>,
                                                                            const VkRenderPass);

        using ShaderModules = std::vector<ShaderModuleConfiguration>();

        using DescriptorPool = DescriptorPoolConfiguration();

        using DescriptorSetLayout = DescriptorSetLayoutConfiguration();

        using DescriptorSets = DescriptorSetsConfiguration(const VkDescriptorSetLayout, const VkDescriptorPool);

        using PipelineLayout = PipelineLayoutConfiguration(const VkDescriptorSetLayout);

        const std::function<Instance>              getInstance;
        const std::function<PhysicalDevice>        getPhysicalDevice;
        const std::function<Device>                getDevice;
        const std::function<Swapchain>             getSwapchain;
        const std::function<RenderPass>            getRenderPass;
        const std::function<SwapchainImageViews>   getSwapchainImageViews;
        const std::function<SwapchainFramebuffers> getSwapchainFramebuffers;
        const std::function<ShaderModules>         getShaderModules;
        const std::function<DescriptorPool>        getDescriptorPool;
        const std::function<DescriptorSetLayout>   getDescriptorSetLayout;
        const std::function<DescriptorSets>        getDescriptorSets;
        const std::function<PipelineLayout>        getPipelineLayout;
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

    std::vector<ImageViewConfiguration>
    getSwapchainImageViewConfigurations(const SwapchainConfiguration& swapchainConfiguration, const std::vector<Image>& images) noexcept;

    std::vector<FramebufferConfiguration>
    getSwapchainFramebufferConfigurations(const SwapchainConfiguration& swapchainConfiguration,
                                          const std::vector<ImageView>  imageViews,
                                          const VkRenderPass            renderPass) noexcept;

    std::vector<ShaderModuleConfiguration>
    getShaderModulesConfigurations() noexcept;

    DescriptorPoolConfiguration
    getDescriptorPoolConfiguration() noexcept;

    DescriptorSetLayoutConfiguration
    getDescriptorSetLayoutConfiguration() noexcept;

    DescriptorSetsConfiguration
    getDescriptorSetsConfiguration(const VkDescriptorSetLayout descriptorSetLayout, const VkDescriptorPool descriptorPool) noexcept;

    PipelineLayoutConfiguration
    getPipelineLayoutConfiguration(const VkDescriptorSetLayout descriptorSetLayout) noexcept;

    class VulkanContextInitializersBuilder final
    {
    public:
        using Type    = VulkanContextInitializers;
        using Builder = VulkanContextInitializersBuilder;

        Type
        build() const;

        operator Type() const noexcept
        {
            return build();
        }

        Builder&
        add(decltype(Type::getInstance)& initializer) noexcept
        {
            getInstance = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getPhysicalDevice)& initializer) noexcept
        {
            getPhysicalDevice = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getDevice)& initializer) noexcept
        {
            getDevice = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getSurface)& initializer) noexcept
        {
            getSurface = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getSwapchain)& initializer) noexcept
        {
            getSwapchain = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getRenderPass)& initializer) noexcept
        {
            getRenderPass = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getSwapchainImages)& initializer) noexcept
        {
            getSwapchainImages = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getSwapchainImageViews)& initializer) noexcept
        {
            getSwapchainImageViews = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getSwapchainFramebuffers)& initializer) noexcept
        {
            getSwapchainFramebuffers = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getShaderModules)& initializer) noexcept
        {
            getShaderModules = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getDescriptorPool)& initializer) noexcept
        {
            getDescriptorPool = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getDescriptorSetLayout)& initializer) noexcept
        {
            getDescriptorSetLayout = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getDescriptorSets)& initializer) noexcept
        {
            getDescriptorSets = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getPipelineLayout)& initializer) noexcept
        {
            getPipelineLayout = initializer;

            return *this;
        }

        Builder&
        operator<<(decltype(Type::getInstance)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getPhysicalDevice)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getDevice)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getSurface)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getSwapchain)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getRenderPass)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getSwapchainImages)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getSwapchainImageViews)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getSwapchainFramebuffers)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getShaderModules)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getDescriptorPool)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getDescriptorSetLayout)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getDescriptorSets)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getPipelineLayout)& initializer) noexcept
        {
            return add(initializer);
        }

    private:
        std::remove_cv_t<decltype(Type::getInstance)>              getInstance;
        std::remove_cv_t<decltype(Type::getPhysicalDevice)>        getPhysicalDevice;
        std::remove_cv_t<decltype(Type::getDevice)>                getDevice;
        std::remove_cv_t<decltype(Type::getSurface)>               getSurface;
        std::remove_cv_t<decltype(Type::getSwapchain)>             getSwapchain;
        std::remove_cv_t<decltype(Type::getRenderPass)>            getRenderPass;
        std::remove_cv_t<decltype(Type::getSwapchainImages)>       getSwapchainImages;
        std::remove_cv_t<decltype(Type::getSwapchainImageViews)>   getSwapchainImageViews;
        std::remove_cv_t<decltype(Type::getSwapchainFramebuffers)> getSwapchainFramebuffers;
        std::remove_cv_t<decltype(Type::getShaderModules)>         getShaderModules;
        std::remove_cv_t<decltype(Type::getDescriptorPool)>        getDescriptorPool;
        std::remove_cv_t<decltype(Type::getDescriptorSetLayout)>   getDescriptorSetLayout;
        std::remove_cv_t<decltype(Type::getDescriptorSets)>        getDescriptorSets;
        std::remove_cv_t<decltype(Type::getPipelineLayout)>        getPipelineLayout;
    };

    class VulkanContextConfigurationsBuilder final
    {
    public:
        using Type    = VulkanContextConfigurations;
        using Builder = VulkanContextConfigurationsBuilder;

        Type
        build() const;

        operator Type() const noexcept
        {
            return build();
        }

        Builder&
        add(decltype(Type::getInstance)& configuration) noexcept
        {
            getInstance = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getPhysicalDevice)& configuration) noexcept
        {
            getPhysicalDevice = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getDevice)& configuration) noexcept
        {
            getDevice = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getSwapchain)& configuration) noexcept
        {
            getSwapchain = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getRenderPass)& configuration) noexcept
        {
            getRenderPass = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getSwapchainImageViews)& configuration) noexcept
        {
            getSwapchainImageViews = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getSwapchainFramebuffers)& configuration) noexcept
        {
            getSwapchainFramebuffers = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getShaderModules)& configuration) noexcept
        {
            getShaderModules = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getDescriptorPool)& configuration) noexcept
        {
            getDescriptorPool = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getDescriptorSetLayout)& configuration) noexcept
        {
            getDescriptorSetLayout = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getDescriptorSets)& configuration) noexcept
        {
            getDescriptorSets = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getPipelineLayout)& configuration) noexcept
        {
            getPipelineLayout = configuration;

            return *this;
        }

        Builder&
        operator<<(decltype(Type::getInstance)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getPhysicalDevice)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getDevice)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getSwapchain)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getRenderPass)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getSwapchainImageViews)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getSwapchainFramebuffers)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getShaderModules)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getDescriptorPool)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getDescriptorSetLayout)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getDescriptorSets)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getPipelineLayout)& configuration) noexcept
        {
            return add(configuration);
        }

    private:
        std::remove_cv_t<decltype(Type::getInstance)>              getInstance;
        std::remove_cv_t<decltype(Type::getPhysicalDevice)>        getPhysicalDevice;
        std::remove_cv_t<decltype(Type::getDevice)>                getDevice;
        std::remove_cv_t<decltype(Type::getSwapchain)>             getSwapchain;
        std::remove_cv_t<decltype(Type::getRenderPass)>            getRenderPass;
        std::remove_cv_t<decltype(Type::getSwapchainImageViews)>   getSwapchainImageViews;
        std::remove_cv_t<decltype(Type::getSwapchainFramebuffers)> getSwapchainFramebuffers;
        std::remove_cv_t<decltype(Type::getShaderModules)>         getShaderModules;
        std::remove_cv_t<decltype(Type::getDescriptorPool)>        getDescriptorPool;
        std::remove_cv_t<decltype(Type::getDescriptorSetLayout)>   getDescriptorSetLayout;
        std::remove_cv_t<decltype(Type::getDescriptorSets)>        getDescriptorSets;
        std::remove_cv_t<decltype(Type::getPipelineLayout)>        getPipelineLayout;
    };

    extern VulkanContextInitializersBuilder   initializersBuilder;
    extern VulkanContextConfigurationsBuilder configurationsBuilder;
} // namespace nd::src::graphics::vulkan
