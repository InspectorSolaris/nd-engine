#pragma once

#include "pch.hpp"

#include "context_configurations.hpp"
#include "context_initializers.hpp"

namespace nd::src::graphics::vulkan
{
    struct VulkanObjects final
    {
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

    class VulkanContext final
    {
    public:
        VulkanContext(VulkanObjects&& objects);

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
        VulkanObjects objects_ {};
    };

    VulkanContext
    getVulkanContext(const VulkanContextConfigurationExternal& configurationExternal,
                     const VulkanContextInitializers&          initializers,
                     const VulkanContextConfigurations&        configurations);
} // namespace nd::src::graphics::vulkan
