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
            const Device    device;
            const Swapchain swapchain;

            const std::vector<Image>&        swapchainImages;
            const std::vector<ImageView>&    swapchainImageViews;
            const std::vector<Framebuffer>&  swapchainFramebuffers;
            const std::vector<ShaderModule>& shaderModules;
            const std::vector<CommandPool>&  commandPools;

            const Pipelines&      pipelines;
            const DescriptorSets& descriptorSets;
            const CommandBuffers& commandBuffers;

            const std::vector<VkSemaphore>& imageAcquiredSemaphores;
            const std::vector<VkSemaphore>& imageRenderedSemaphores;
            const std::vector<VkFence>&     imageAcquiredFences;
            const std::vector<VkFence>&     imageRenderedFences;

            const size_t framesCount;

            const Instance            instance;
            const Surface             surface;
            const RenderPass          renderPass;
            const DescriptorPool      descriptorPool;
            const DescriptorSetLayout descriptorSetLayout;
            const PipelineLayout      pipelineLayout;
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
        Device    device_ {};
        Swapchain swapchain_ {};

        std::vector<Image>        swapchainImages_ {};
        std::vector<ImageView>    swapchainImageViews_ {};
        std::vector<Framebuffer>  swapchainFramebuffers_ {};
        std::vector<ShaderModule> shaderModules_ {};
        std::vector<CommandPool>  commandPools_ {};

        Pipelines      pipelines_ {};
        DescriptorSets descriptorSets_ {};
        CommandBuffers commandBuffers_ {};

        std::vector<VkSemaphore> imageAcquiredSemaphores_ {};
        std::vector<VkSemaphore> imageRenderedSemaphores_ {};
        std::vector<VkFence>     imageAcquiredFences_ {};
        std::vector<VkFence>     imageRenderedFences_ {};

        size_t framesCount_ {};

        Instance            instance_ {};
        Surface             surface_ {};
        RenderPass          renderPass_ {};
        DescriptorPool      descriptorPool_ {};
        DescriptorSetLayout descriptorSetLayout_ {};
        PipelineLayout      pipelineLayout_ {};
    };

    VulkanContext
    getVulkanContext(const VulkanContextConfigurationExternal& configurationExternal,
                     const VulkanContextInitializers&          initializers,
                     const VulkanContextConfigurations&        configurations);
} // namespace nd::src::graphics::vulkan
