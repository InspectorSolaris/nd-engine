#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct Vertex final
    {
        glm::vec3 position;
        glm::vec3 color;
    };

    struct VulkanConfigurationExternal final
    {
        const std::string& applicationName;
        const std::string& engineName;

        const std::vector<std::string>& layers;
        const std::vector<std::string>& extensions;

        const uint32_t width;
        const uint32_t height;
    };

    struct VulkanConfigurations final
    {
        using InstanceFunction = InstanceConfiguration(const VulkanConfigurationExternal&);

        using PhysicalDeviceFunction = PhysicalDeviceConfiguration();

        using DeviceFunction = DeviceConfiguration(const PhysicalDeviceConfiguration&);

        using DeviceMemoriesFunction = std::vector<DeviceMemoryConfiguration>(const VkPhysicalDeviceMemoryProperties*);

        using SwapchainFunction = SwapchainConfiguration(const VkPhysicalDevice, const VkSurfaceKHR, const uint32_t, const uint32_t);

        using RenderPassFunction = RenderPassConfiguration(const SwapchainConfiguration&);

        using SwapchainImageViewsFunction = std::vector<ImageViewConfiguration>(const SwapchainConfiguration&, const std::vector<Image>&);

        using SwapchainFramebuffersFunction = std::vector<FramebufferConfiguration>(const SwapchainConfiguration&,
                                                                                    const std::vector<ImageView>&,
                                                                                    const VkRenderPass);

        using ShaderModulesFunction = std::vector<ShaderModuleConfiguration>();

        using DescriptorPoolFunction = DescriptorPoolConfiguration();

        using DescriptorSetLayoutsFunction = std::vector<DescriptorSetLayoutConfiguration>();

        using DescriptorSetFunction = DescriptorSetConfiguration(const std::vector<DescriptorSetLayout>&, const VkDescriptorPool);

        using PipelineLayoutsFunction = std::vector<PipelineLayoutConfiguration>(const std::vector<DescriptorSetLayout>&);

        using GraphicsPipelinesFunction = std::vector<GraphicsPipelineConfiguration>(const std::vector<ShaderModule>&,
                                                                                     const std::vector<PipelineLayout>&,
                                                                                     const VkRenderPass,
                                                                                     const uint32_t,
                                                                                     const uint32_t);

        using CommandPoolsFunction = std::vector<CommandPoolConfiguration>(const std::vector<QueueFamily>&);

        using CommandBuffersFunction = std::vector<CommandBufferConfiguration>(const std::vector<CommandPool>&);

        using BuffersFunction = std::vector<BufferConfiguration>(const std::vector<QueueFamily>&);

        const std::function<InstanceFunction>              getInstance;
        const std::function<PhysicalDeviceFunction>        getPhysicalDevice;
        const std::function<DeviceFunction>                getDevice;
        const std::function<DeviceMemoriesFunction>        getDeviceMemories;
        const std::function<SwapchainFunction>             getSwapchain;
        const std::function<RenderPassFunction>            getRenderPass;
        const std::function<SwapchainImageViewsFunction>   getSwapchainImageViews;
        const std::function<SwapchainFramebuffersFunction> getSwapchainFramebuffers;
        const std::function<ShaderModulesFunction>         getShaderModules;
        const std::function<DescriptorPoolFunction>        getDescriptorPool;
        const std::function<DescriptorSetLayoutsFunction>  getDescriptorSetLayouts;
        const std::function<DescriptorSetFunction>         getDescriptorSet;
        const std::function<PipelineLayoutsFunction>       getPipelineLayouts;
        const std::function<GraphicsPipelinesFunction>     getGraphicsPipelines;
        const std::function<CommandPoolsFunction>          getCommandPools;
        const std::function<CommandBuffersFunction>        getCommandBuffers;
        const std::function<BuffersFunction>               getBuffers;
    };

    InstanceConfiguration
    getInstanceConfiguration(const VulkanConfigurationExternal& configurationExternal) noexcept;

    PhysicalDeviceConfiguration
    getPhysicalDeviceConfiguration() noexcept;

    DeviceConfiguration
    getDeviceConfiguration(const PhysicalDeviceConfiguration& physicalDeviceConfiguration) noexcept;

    std::vector<DeviceMemoryConfiguration>
    getDeviceMemoryConfigurations(const VkPhysicalDeviceMemoryProperties* physicalDeviceMemoryProperties) noexcept;

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
                                          const std::vector<ImageView>& imageViews,
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

    std::vector<BufferConfiguration>
    getBufferConfigurations(const std::vector<QueueFamily>& queueFamiliesPool);
} // namespace nd::src::graphics::vulkan
