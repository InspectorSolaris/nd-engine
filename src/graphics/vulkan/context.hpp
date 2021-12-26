#pragma once

#include "pch.hpp"
#include "shared.hpp"

#include "context_configuration.hpp"

namespace nd::src::graphics::vulkan
{
    class VulkanContext final
    {
    public:
        struct Configuration final
        {
            const Device    device;
            const Swapchain swapchain;

            const std::vector<VkImage>&         swapchainImages;
            const std::vector<VkImageView>&     swapchainImageViews;
            const std::vector<VkFramebuffer>&   swapchainFramebuffers;
            const std::vector<ShaderModule>&    shaderModules;
            const std::vector<VkDescriptorSet>& descriptorSets;
            const std::vector<VkPipeline>&      pipelines;
            const std::vector<VkCommandBuffer>& commandBuffers;

            const std::vector<VkSemaphore>& imageAcquiredSemaphores;
            const std::vector<VkSemaphore>& imageRenderedSemaphores;
            const std::vector<VkFence>&     imageAcquiredFences;
            const std::vector<VkFence>&     imageRenderedFences;

            const size_t framesCount;

            const VkInstance            instance;
            const VkSurfaceKHR          surface;
            const VkRenderPass          renderPass;
            const VkDescriptorPool      descriptorPool;
            const VkDescriptorSetLayout descriptorSetLayout;
            const VkPipelineLayout      pipelineLayout;
            const VkCommandPool         commandPool;
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

        std::vector<VkImage>         swapchainImages_ {};
        std::vector<VkImageView>     swapchainImageViews_ {};
        std::vector<VkFramebuffer>   swapchainFramebuffers_ {};
        std::vector<ShaderModule>    shaderModules_ {};
        std::vector<VkDescriptorSet> descriptorSets_ {};
        std::vector<VkPipeline>      pipelines_ {};
        std::vector<VkCommandBuffer> commandBuffers_ {};

        std::vector<VkSemaphore> imageAcquiredSemaphores_ {};
        std::vector<VkSemaphore> imageRenderedSemaphores_ {};
        std::vector<VkFence>     imageAcquiredFences_ {};
        std::vector<VkFence>     imageRenderedFences_ {};

        size_t framesCount_ {};

        VkInstance            instance_ {};
        VkSurfaceKHR          surface_ {};
        VkRenderPass          renderPass_ {};
        VkDescriptorPool      descriptorPool_ {};
        VkDescriptorSetLayout descriptorSetLayout_ {};
        VkPipelineLayout      pipelineLayout_ {};
        VkCommandPool         commandPool_ {};
    };

    VulkanContext
    getVulkanContext(const VulkanContextConfigurationExternal& configurationExternal,
                     const VulkanContextInitializers&          initializers,
                     const VulkanContextConfigurations&        configurations);
} // namespace nd::src::graphics::vulkan
