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

        using DescriptorSetLayouts = std::vector<DescriptorSetLayoutConfiguration>();

        using DescriptorSet = DescriptorSetConfiguration(const std::vector<DescriptorSetLayout>&, const VkDescriptorPool);

        using PipelineLayouts = std::vector<PipelineLayoutConfiguration>(const std::vector<DescriptorSetLayout>&);

        using GraphicsPipelines = std::vector<GraphicsPipelineConfiguration>(const std::vector<ShaderModule>&,
                                                                             const std::vector<PipelineLayout>&,
                                                                             const VkRenderPass,
                                                                             const uint32_t,
                                                                             const uint32_t);

        using CommandPools = std::vector<CommandPoolConfiguration>(const std::vector<QueueFamily>&);

        using CommandBuffers = std::vector<CommandBufferConfiguration>(const std::vector<CommandPool>&);

        const std::function<Instance>              getInstance;
        const std::function<PhysicalDevice>        getPhysicalDevice;
        const std::function<Device>                getDevice;
        const std::function<Swapchain>             getSwapchain;
        const std::function<RenderPass>            getRenderPass;
        const std::function<SwapchainImageViews>   getSwapchainImageViews;
        const std::function<SwapchainFramebuffers> getSwapchainFramebuffers;
        const std::function<ShaderModules>         getShaderModules;
        const std::function<DescriptorPool>        getDescriptorPool;
        const std::function<DescriptorSetLayouts>  getDescriptorSetLayouts;
        const std::function<DescriptorSet>         getDescriptorSet;
        const std::function<PipelineLayouts>       getPipelineLayouts;
        const std::function<GraphicsPipelines>     getGraphicsPipelines;
        const std::function<CommandPools>          getCommandPools;
        const std::function<CommandBuffers>        getCommandBuffers;
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

    std::vector<ImageViewConfiguration>
    getSwapchainImageViewConfigurations(const SwapchainConfiguration& swapchainConfiguration, const std::vector<Image>& images) noexcept;

    FramebufferConfiguration
    getSwapchainFramebufferConfiguration(const SwapchainConfiguration& swapchainConfiguration,
                                         const VkImageView             imageView,
                                         const VkRenderPass            renderPass) noexcept;

    std::vector<FramebufferConfiguration>
    getSwapchainFramebufferConfigurations(const SwapchainConfiguration& swapchainConfiguration,
                                          const std::vector<ImageView>  imageViews,
                                          const VkRenderPass            renderPass) noexcept;

    std::vector<ShaderModuleConfiguration>
    getShaderModulesConfigurations() noexcept;

    DescriptorPoolConfiguration
    getDescriptorPoolConfiguration() noexcept;

    std::vector<DescriptorSetLayoutConfiguration>
    getDescriptorSetLayoutConfigurations() noexcept;

    DescriptorSetConfiguration
    getDescriptorSetConfiguration(const std::vector<DescriptorSetLayout>& descriptorSetLayouts, const VkDescriptorPool descriptorPool) noexcept;

    std::vector<PipelineLayoutConfiguration>
    getPipelineLayoutConfigurations(const std::vector<DescriptorSetLayout>& descriptorSetLayouts) noexcept;

    std::vector<GraphicsPipelineConfiguration>
    getGraphicsPipelineConfigurations(const std::vector<ShaderModule>&   shaderModules,
                                      const std::vector<PipelineLayout>& pipelineLayouts,
                                      const VkRenderPass                 renderPass,
                                      const uint32_t                     width,
                                      const uint32_t                     height) noexcept;

    std::vector<CommandPoolConfiguration>
    getCommandPoolConfigurations(const std::vector<QueueFamily>& queueFamiliesPool);

    std::vector<CommandBufferConfiguration>
    getCommandBufferConfigurations(const std::vector<CommandPool>& commandPools) noexcept;

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
        add(decltype(Type::getDescriptorSetLayouts)& configuration) noexcept
        {
            getDescriptorSetLayouts = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getDescriptorSet)& configuration) noexcept
        {
            getDescriptorSet = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getPipelineLayouts)& configuration) noexcept
        {
            getPipelineLayouts = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getGraphicsPipelines)& configuration) noexcept
        {
            getGraphicsPipelines = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getCommandPools)& configuration) noexcept
        {
            getCommandPools = configuration;

            return *this;
        }

        Builder&
        add(decltype(Type::getCommandBuffers)& configuration) noexcept
        {
            getCommandBuffers = configuration;

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
        operator<<(decltype(Type::getDescriptorSetLayouts)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getDescriptorSet)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getPipelineLayouts)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getGraphicsPipelines)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getCommandPools)& configuration) noexcept
        {
            return add(configuration);
        }

        Builder&
        operator<<(decltype(Type::getCommandBuffers)& configuration) noexcept
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
        std::remove_cv_t<decltype(Type::getDescriptorSetLayouts)>  getDescriptorSetLayouts;
        std::remove_cv_t<decltype(Type::getDescriptorSet)>         getDescriptorSet;
        std::remove_cv_t<decltype(Type::getPipelineLayouts)>       getPipelineLayouts;
        std::remove_cv_t<decltype(Type::getGraphicsPipelines)>     getGraphicsPipelines;
        std::remove_cv_t<decltype(Type::getCommandPools)>          getCommandPools;
        std::remove_cv_t<decltype(Type::getCommandBuffers)>        getCommandBuffers;
    };

    extern VulkanContextConfigurationsBuilder configurationsBuilder;
} // namespace nd::src::graphics::vulkan
