#pragma once

#include "pch.hpp"

#include "context_configurations.hpp"
#include "context_initializers.hpp"

namespace nd::src::graphics::vulkan
{
    struct VulkanObjects final
    {
        VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties {};

        Device    device {};
        Swapchain swapchain {};

        std::vector<Image>               swapchainImages {};
        std::vector<ImageView>           swapchainImageViews {};
        std::vector<Framebuffer>         swapchainFramebuffers {};
        std::vector<ShaderModule>        shaderModules {};
        std::vector<DescriptorSetLayout> descriptorSetLayouts {};
        std::vector<DescriptorSet>       descriptorSets {};
        std::vector<PipelineLayout>      pipelineLayouts {};
        std::vector<Pipeline>            graphicsPipelines {};
        std::vector<CommandPool>         commandPools {};
        std::vector<CommandBuffers>      commandBuffers {};
        std::vector<Buffer>              buffers {};
        std::vector<DeviceMemories>      bufferMemories {};

        std::vector<VkSemaphore> imageAcquiredSemaphores {};
        std::vector<VkSemaphore> imageRenderedSemaphores {};
        std::vector<VkFence>     imageAcquiredFences {};
        std::vector<VkFence>     imageRenderedFences {};

        size_t framesCount {};

        Instance       instance {};
        PhysicalDevice physicalDevice {};
        Surface        surface {};
        RenderPass     renderPass {};
        DescriptorPool descriptorPool {};
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
