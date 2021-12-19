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

namespace nd::src::graphics::vulkan
{
    struct ContextConfiguration final
    {
        const std::function<VkSurfaceKHR(const VkInstance instance)>& getSurface;

        const std::string& applicationName;
        const std::string& engineName;

        const std::vector<std::string>& layers;
        const std::vector<std::string>& extensions;

        const uint32_t width;
        const uint32_t height;
    };

    class Context final
    {
    public:
        struct Configuration final
        {
            const std::vector<VkImage>&&         swapchainImages;
            const std::vector<VkImageView>&&     swapchainImageViews;
            const std::vector<VkFramebuffer>&&   swapchainFramebuffers;
            const std::vector<VkShaderModule>&&  shaderModules;
            const std::vector<VkDescriptorSet>&& descriptorSets;
            const std::vector<VkPipeline>&&      pipelines;
            const std::vector<VkCommandBuffer>&& commandBuffers;

            const VkInstance            instance;
            const VkDevice              device;
            const VkSurfaceKHR          surface;
            const VkSwapchainKHR        swapchain;
            const VkRenderPass          renderPass;
            const VkDescriptorPool      descriptorPool;
            const VkDescriptorSetLayout descriptorSetLayout;
            const VkPipelineLayout      pipelineLayout;
            const VkCommandPool         commandPool;
        };

        Context(const Configuration& configuration);

        Context(const Context& vulkanContext) = delete;
        Context(Context&& vulkanContext)      = delete;

        Context&
        operator=(const Context& vulkanContext) = delete;
        Context&
        operator=(Context&& vulkanContext) = delete;

        ~Context();

    private:
        std::vector<VkImage>         swapchainImages_ {};
        std::vector<VkImageView>     swapchainImageViews_ {};
        std::vector<VkFramebuffer>   swapchainFramebuffers_ {};
        std::vector<VkShaderModule>  shaderModules_ {};
        std::vector<VkDescriptorSet> descriptorSets_ {};
        std::vector<VkPipeline>      pipelines_ {};
        std::vector<VkCommandBuffer> commandBuffers_ {};

        VkInstance            instance_ {};
        VkDevice              device_ {};
        VkSurfaceKHR          surface_ {};
        VkSwapchainKHR        swapchain_ {};
        VkRenderPass          renderPass_ {};
        VkDescriptorPool      descriptorPool_ {};
        VkDescriptorSetLayout descriptorSetLayout_ {};
        VkPipelineLayout      pipelineLayout_ {};
        VkCommandPool         commandPool_ {};
    };

    Context
    getContext(const ContextConfiguration& configuration);
} // namespace nd::src::graphics::vulkan
