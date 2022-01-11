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

        using Buffers = std::vector<BufferConfiguration>(const std::vector<QueueFamily>&);

        using BufferMemories = std::vector<DeviceMemoryConfiguration>(const VkPhysicalDeviceMemoryProperties&, const VkMemoryRequirements);

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
        const std::function<Buffers>               getBuffers;
        const std::function<BufferMemories>        getBufferMemories;
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

    std::vector<BufferConfiguration>
    getBufferConfigurations(const std::vector<QueueFamily>& queueFamiliesPool);

    std::vector<DeviceMemoryConfiguration>
    getBufferMemoryConfigurations(const VkPhysicalDeviceMemoryProperties& memoryProperties, const VkMemoryRequirements memoryRequirements) noexcept;
} // namespace nd::src::graphics::vulkan
