#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct VulkanInitializers final
    {
        using Instance = Instance(const InstanceConfiguration&);

        using PhysicalDevice = PhysicalDevice(const PhysicalDeviceConfiguration&, const VkInstance);

        using Device = Device(const DeviceConfiguration&, const VkPhysicalDevice);

        using DeviceMemories = std::vector<DeviceMemory>(const std::vector<DeviceMemoryConfiguration>&, const VkDevice);

        using Surface = Surface(const VkInstance);

        using Swapchain = Swapchain(const SwapchainConfiguration&, const VkDevice);

        using RenderPass = RenderPass(const RenderPassConfiguration&, const VkDevice);

        using SwapchainImages = std::vector<Image>(const VkSwapchainKHR, const VkDevice);

        using SwapchainImageViews = std::vector<ImageView>(const std::vector<ImageViewConfiguration>&, const VkDevice);

        using SwapchainFramebuffers = std::vector<Framebuffer>(const std::vector<FramebufferConfiguration>&, const VkDevice);

        using ShaderModules = std::vector<ShaderModule>(const std::vector<ShaderModuleConfiguration>&, const VkDevice);

        using DescriptorPool = DescriptorPool(const DescriptorPoolConfiguration&, const VkDevice);

        using DescriptorSetLayouts = std::vector<DescriptorSetLayout>(const std::vector<DescriptorSetLayoutConfiguration>&, const VkDevice);

        using DescriptorSets = std::vector<DescriptorSet>(const DescriptorSetConfiguration&, const VkDevice);

        using PipelineLayouts = std::vector<PipelineLayout>(const std::vector<PipelineLayoutConfiguration>&, const VkDevice);

        using GraphicsPipelines = std::vector<Pipeline>(const std::vector<GraphicsPipelineConfiguration>&, const VkDevice);

        using CommandPools = std::vector<CommandPool>(const std::vector<CommandPoolConfiguration>&, const VkDevice);

        using CommandBuffers = std::vector<CommandBuffers>(const std::vector<CommandBufferConfiguration>&, const VkDevice);

        using Buffers = std::vector<Buffer>(const std::vector<BufferConfiguration>&, const VkDevice);

        using DeviceMemoryBufferBindings = std::vector<size_t>(const VkDevice,
                                                               const VkPhysicalDeviceMemoryProperties*,
                                                               const std::vector<DeviceMemory>&,
                                                               const std::vector<Buffer>&);

        using DeviceMemoryImageBindings = std::vector<size_t>(const VkDevice,
                                                              const VkPhysicalDeviceMemoryProperties*,
                                                              const std::vector<DeviceMemory>&,
                                                              const std::vector<Image>&);

        const std::function<Instance>                   getInstance;
        const std::function<PhysicalDevice>             getPhysicalDevice;
        const std::function<Device>                     getDevice;
        const std::function<DeviceMemories>             getDeviceMemories;
        const std::function<Surface>                    getSurface;
        const std::function<Swapchain>                  getSwapchain;
        const std::function<RenderPass>                 getRenderPass;
        const std::function<SwapchainImages>            getSwapchainImages;
        const std::function<SwapchainImageViews>        getSwapchainImageViews;
        const std::function<SwapchainFramebuffers>      getSwapchainFramebuffers;
        const std::function<ShaderModules>              getShaderModules;
        const std::function<DescriptorPool>             getDescriptorPool;
        const std::function<DescriptorSetLayouts>       getDescriptorSetLayouts;
        const std::function<DescriptorSets>             getDescriptorSets;
        const std::function<PipelineLayouts>            getPipelineLayouts;
        const std::function<GraphicsPipelines>          getGraphicsPipelines;
        const std::function<CommandPools>               getCommandPools;
        const std::function<CommandBuffers>             getCommandBuffers;
        const std::function<Buffers>                    getBuffers;
        const std::function<DeviceMemoryBufferBindings> bindBufferMemory;
        const std::function<DeviceMemoryImageBindings>  bindImageMemory;
    };
} // namespace nd::src::graphics::vulkan
