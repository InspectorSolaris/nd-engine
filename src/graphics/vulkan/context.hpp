#pragma once

#include "pch.hpp"

#include "context_configurations.hpp"
#include "context_initializers.hpp"

namespace nd::src::graphics::vulkan
{
    class VulkanContext final
    {
    public:
        struct Configuration final
        {
            const VkPhysicalDeviceMemoryProperties& physicalDeviceMemoryProperties;

            const Device    device;
            const Swapchain swapchain;

            const std::vector<Image>&               swapchainImages;
            const std::vector<ImageView>&           swapchainImageViews;
            const std::vector<Framebuffer>&         swapchainFramebuffers;
            const std::vector<ShaderModule>&        shaderModules;
            const std::vector<DescriptorSetLayout>& descriptorSetLayouts;
            const std::vector<DescriptorSet>        descriptorSets;
            const std::vector<PipelineLayout>&      pipelineLayouts;
            const std::vector<Pipeline>&            graphicsPipelines;
            const std::vector<CommandPool>&         commandPools;
            const std::vector<CommandBuffers>&      commandBuffers;

            const std::vector<VkSemaphore>& imageAcquiredSemaphores;
            const std::vector<VkSemaphore>& imageRenderedSemaphores;
            const std::vector<VkFence>&     imageAcquiredFences;
            const std::vector<VkFence>&     imageRenderedFences;

            const size_t framesCount;

            const Instance       instance;
            const PhysicalDevice physicalDevice;
            const Surface        surface;
            const RenderPass     renderPass;
            const DescriptorPool descriptorPool;
        };

        VulkanContext(const Configuration& configuration);

        VulkanContext(const VulkanContext& vulkanContext) = delete;
        VulkanContext(VulkanContext&& vulkanContext)      = delete;

        VulkanContext&
        operator=(const VulkanContext& vulkanContext) = delete;
        VulkanContext&
        operator=(VulkanContext&& vulkanContext) = delete;

        ~VulkanContext();

        void
        drawNextFrame();

    private:
        VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties_ {};

        Device    device_ {};
        Swapchain swapchain_ {};

        std::vector<Image>               swapchainImages_ {};
        std::vector<ImageView>           swapchainImageViews_ {};
        std::vector<Framebuffer>         swapchainFramebuffers_ {};
        std::vector<ShaderModule>        shaderModules_ {};
        std::vector<DescriptorSetLayout> descriptorSetLayouts_ {};
        std::vector<DescriptorSet>       descriptorSets_ {};
        std::vector<PipelineLayout>      pipelineLayouts_ {};
        std::vector<Pipeline>            graphicsPipelines_ {};
        std::vector<CommandPool>         commandPools_ {};
        std::vector<CommandBuffers>      commandBuffers_ {};
        std::vector<Buffer>              buffers_ {};
        std::vector<DeviceMemories>      bufferMemories_ {};

        std::vector<VkSemaphore> imageAcquiredSemaphores_ {};
        std::vector<VkSemaphore> imageRenderedSemaphores_ {};
        std::vector<VkFence>     imageAcquiredFences_ {};
        std::vector<VkFence>     imageRenderedFences_ {};

        size_t framesCount_ {};

        Instance       instance_ {};
        PhysicalDevice physicalDevice_ {};
        Surface        surface_ {};
        RenderPass     renderPass_ {};
        DescriptorPool descriptorPool_ {};
    };

    VulkanContext
    getVulkanContext(const VulkanContextConfigurationExternal& configurationExternal,
                     const VulkanContextInitializers&          initializers,
                     const VulkanContextConfigurations&        configurations);
} // namespace nd::src::graphics::vulkan
