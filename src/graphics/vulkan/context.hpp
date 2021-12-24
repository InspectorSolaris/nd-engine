#pragma once

#include "pch.hpp"
#include "shared.hpp"

#include "instance.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "swapchain.hpp"
#include "descriptor_pool.hpp"
#include "descriptor_set_layout.hpp"
#include "descriptor_set.hpp"
#include "render_pass.hpp"
#include "shader_module.hpp"
#include "pipeline_layout.hpp"
#include "pipeline.hpp"
#include "image.hpp"
#include "image_view.hpp"
#include "framebuffer.hpp"
#include "buffer.hpp"
#include "buffer_view.hpp"
#include "command_pool.hpp"
#include "command_buffer.hpp"
#include "synchronization.hpp"

namespace nd::src::graphics::vulkan
{
    struct VulkanContextConfiguration final
    {
        const std::function<VkSurfaceKHR(const VkInstance instance)>& getSurface;

        const std::string& applicationName;
        const std::string& engineName;

        const std::vector<std::string>& layers;
        const std::vector<std::string>& extensions;

        const uint32_t width;
        const uint32_t height;
    };

    struct VulkanContextCreaters final
    {
    };

    class VulkanContext final
    {
    public:
        struct Configuration final
        {
            std::vector<VkImage>&&         swapchainImages;
            std::vector<VkImageView>&&     swapchainImageViews;
            std::vector<VkFramebuffer>&&   swapchainFramebuffers;
            std::vector<VkShaderModule>&&  shaderModules;
            std::vector<VkDescriptorSet>&& descriptorSets;
            std::vector<VkPipeline>&&      pipelines;
            std::vector<VkCommandBuffer>&& commandBuffers;

            std::vector<VkSemaphore>&& imageAcquiredSemaphores;
            std::vector<VkSemaphore>&& imageRenderedSemaphores;
            std::vector<VkFence>&&     imageAcquiredFences;
            std::vector<VkFence>&&     imageRenderedFences;

            const size_t framesCount;

            const VkInstance            instance;
            const VkDevice              device;
            const VkSurfaceKHR          surface;
            const VkSwapchainKHR        swapchain;
            const VkRenderPass          renderPass;
            const VkDescriptorPool      descriptorPool;
            const VkDescriptorSetLayout descriptorSetLayout;
            const VkPipelineLayout      pipelineLayout;
            const VkCommandPool         commandPool;

            const VkQueue graphicsQueue;
            const VkQueue presentQueue;
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
        std::vector<VkImage>         swapchainImages_ {};
        std::vector<VkImageView>     swapchainImageViews_ {};
        std::vector<VkFramebuffer>   swapchainFramebuffers_ {};
        std::vector<VkShaderModule>  shaderModules_ {};
        std::vector<VkDescriptorSet> descriptorSets_ {};
        std::vector<VkPipeline>      pipelines_ {};
        std::vector<VkCommandBuffer> commandBuffers_ {};

        std::vector<VkSemaphore> imageAcquiredSemaphores_ {};
        std::vector<VkSemaphore> imageRenderedSemaphores_ {};
        std::vector<VkFence>     imageAcquiredFences_ {};
        std::vector<VkFence>     imageRenderedFences_ {};

        size_t framesCount_ {};

        VkInstance            instance_ {};
        VkDevice              device_ {};
        VkSurfaceKHR          surface_ {};
        VkSwapchainKHR        swapchain_ {};
        VkRenderPass          renderPass_ {};
        VkDescriptorPool      descriptorPool_ {};
        VkDescriptorSetLayout descriptorSetLayout_ {};
        VkPipelineLayout      pipelineLayout_ {};
        VkCommandPool         commandPool_ {};

        VkQueue graphicsQueue_ {};
        VkQueue presentQueue_ {};
    };

    VulkanContext
    getVulkanContext(const VulkanContextConfiguration& configuration);
} // namespace nd::src::graphics::vulkan
