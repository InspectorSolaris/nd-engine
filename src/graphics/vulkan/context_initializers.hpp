#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct VulkanInitializers final
    {
        using InstanceFunction = Instance(const InstanceConfiguration&);

        using PhysicalDeviceFunction = PhysicalDevice(const PhysicalDeviceConfiguration&, const VkInstance);

        using DeviceFunction = Device(const DeviceConfiguration&, const VkPhysicalDevice);

        using DeviceMemoriesFunction = std::vector<DeviceMemory>(const std::vector<DeviceMemoryConfiguration>&, const VkDevice);

        using SurfaceFunction = Surface(const VkInstance);

        using SwapchainFunction = Swapchain(const SwapchainConfiguration&, const VkDevice);

        using RenderPassFunction = RenderPass(const RenderPassConfiguration&, const VkDevice);

        using SwapchainImagesFunction = std::vector<Image>(const VkSwapchainKHR, const VkDevice);

        using SwapchainImageViewsFunction = std::vector<ImageView>(const std::vector<ImageViewConfiguration>&, const VkDevice);

        using SwapchainFramebuffersFunction = std::vector<Framebuffer>(const std::vector<FramebufferConfiguration>&, const VkDevice);

        using ShaderModulesFunction = std::vector<ShaderModule>(const std::vector<ShaderModuleConfiguration>&, const VkDevice);

        using DescriptorPoolFunction = DescriptorPool(const DescriptorPoolConfiguration&, const VkDevice);

        using DescriptorSetLayoutsFunction = std::vector<DescriptorSetLayout>(const std::vector<DescriptorSetLayoutConfiguration>&, const VkDevice);

        using DescriptorSetsFunction = std::vector<DescriptorSet>(const DescriptorSetConfiguration&, const VkDevice);

        using PipelineLayoutsFunction = std::vector<PipelineLayout>(const std::vector<PipelineLayoutConfiguration>&, const VkDevice);

        using GraphicsPipelinesFunction = std::vector<Pipeline>(const std::vector<GraphicsPipelineConfiguration>&, const VkDevice);

        using CommandPoolsFunction = std::vector<CommandPool>(const std::vector<CommandPoolConfiguration>&, const VkDevice);

        using CommandBuffersFunction = std::vector<CommandBuffers>(const std::vector<CommandBufferConfiguration>&, const VkDevice);

        using BuffersFunction = std::vector<Buffer>(const std::vector<BufferConfiguration>&, const VkDevice);

        using DeviceMemoryBufferBindingsFunction = std::vector<size_t>(const VkDevice,
                                                                       const VkPhysicalDeviceMemoryProperties*,
                                                                       const std::vector<DeviceMemory>&,
                                                                       const std::vector<Buffer>&);

        using DeviceMemoryImageBindingsFunction = std::vector<size_t>(const VkDevice,
                                                                      const VkPhysicalDeviceMemoryProperties*,
                                                                      const std::vector<DeviceMemory>&,
                                                                      const std::vector<Image>&);

        const std::function<InstanceFunction>                   getInstance;
        const std::function<PhysicalDeviceFunction>             getPhysicalDevice;
        const std::function<DeviceFunction>                     getDevice;
        const std::function<DeviceMemoriesFunction>             getDeviceMemories;
        const std::function<SurfaceFunction>                    getSurface;
        const std::function<SwapchainFunction>                  getSwapchain;
        const std::function<RenderPassFunction>                 getRenderPass;
        const std::function<SwapchainImagesFunction>            getSwapchainImages;
        const std::function<SwapchainImageViewsFunction>        getSwapchainImageViews;
        const std::function<SwapchainFramebuffersFunction>      getSwapchainFramebuffers;
        const std::function<ShaderModulesFunction>              getShaderModules;
        const std::function<DescriptorPoolFunction>             getDescriptorPool;
        const std::function<DescriptorSetLayoutsFunction>       getDescriptorSetLayouts;
        const std::function<DescriptorSetsFunction>             getDescriptorSets;
        const std::function<PipelineLayoutsFunction>            getPipelineLayouts;
        const std::function<GraphicsPipelinesFunction>          getGraphicsPipelines;
        const std::function<CommandPoolsFunction>               getCommandPools;
        const std::function<CommandBuffersFunction>             getCommandBuffers;
        const std::function<BuffersFunction>                    getBuffers;
        const std::function<DeviceMemoryBufferBindingsFunction> bindBufferMemory;
        const std::function<DeviceMemoryImageBindingsFunction>  bindImageMemory;
    };
} // namespace nd::src::graphics::vulkan
