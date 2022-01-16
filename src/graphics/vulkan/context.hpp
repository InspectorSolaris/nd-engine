#pragma once

#include "pch.hpp"

#include "context_configurations.hpp"
#include "context_initializers.hpp"

namespace nd::src::graphics::vulkan
{
    class VulkanObjects final
    {
    public:
        PhysicalDevice physicalDevice {};
        Device         device {};
        Swapchain      swapchain {};

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

        std::vector<VkSemaphore> semaphores {};
        std::vector<VkFence>     fences {};

        Instance       instance {};
        Surface        surface {};
        RenderPass     renderPass {};
        DescriptorPool descriptorPool {};

        VulkanObjects(Instance&&                         instance,
                      PhysicalDevice&&                   physicalDevice,
                      Device&&                           device,
                      Surface&&                          surface,
                      Swapchain&&                        swapchain,
                      RenderPass&&                       renderPass,
                      std::vector<Image>&&               swapchainImages,
                      std::vector<ImageView>&&           swapchainImageViews,
                      std::vector<Framebuffer>&&         swapchainFramebuffers,
                      std::vector<ShaderModule>&&        shaderModules,
                      DescriptorPool&&                   descriptorPool,
                      std::vector<DescriptorSetLayout>&& descriptorSetLayouts,
                      std::vector<DescriptorSet>&&       descriptorSets,
                      std::vector<PipelineLayout>&&      pipelineLayouts,
                      std::vector<Pipeline>&&            graphicsPipelines,
                      std::vector<CommandPool>&&         commandPools,
                      std::vector<CommandBuffers>&&      commandBuffers,
                      std::vector<Buffer>&&              buffers,
                      std::vector<DeviceMemories>&&      bufferMemories);

        VulkanObjects(const VulkanObjects& vulkanObjects) = delete;
        VulkanObjects(VulkanObjects&& vulkanObjects);

        VulkanObjects&
        operator=(const VulkanObjects& vulkanObjects) = delete;
        VulkanObjects&
        operator=(VulkanObjects&& vulkanObjects);

        ~VulkanObjects();
    };

    class VulkanContext final
    {
    public:
        VulkanContext(VulkanObjects&& objects);

        VulkanContext(const VulkanContext& vulkanContext) = delete;
        VulkanContext(VulkanContext&& vulkanContext);

        VulkanContext&
        operator=(const VulkanContext& vulkanContext) = delete;
        VulkanContext&
        operator=(VulkanContext&& vulkanContext);

        const QueueFamily&
        getGraphicsQueueFamily();

        const QueueFamily&
        getComputeQueueFamily();

        const QueueFamily&
        getTransferQueueFamily();

        const QueueFamily&
        getSwapchainQueueFamily();

        VkSemaphore
        getSemaphore(const VkSemaphoreCreateFlags flags = {}, const void* next = {}) noexcept;

        std::vector<VkSemaphore>
        getSemaphores(const size_t count, const VkSemaphoreCreateFlags flags = {}, const void* next = {}) noexcept;

        VkFence
        getFence(const VkFenceCreateFlags flags = {}, const void* next = {}) noexcept;

        std::vector<VkFence>
        getFences(const size_t count, const VkFenceCreateFlags flags = {}, const void* next = {}) noexcept;

        void
        drawNextFrame();

    private:
        VulkanObjects objects_;
    };

    VulkanObjects
    getVulkanObjects(const VulkanConfigurationExternal& configurationExternal,
                     const VulkanInitializers&          initializers,
                     const VulkanConfigurations&        configurations);

    VulkanContext
    getVulkanContext(const VulkanConfigurationExternal& configurationExternal,
                     const VulkanInitializers&          initializers,
                     const VulkanConfigurations&        configurations);
} // namespace nd::src::graphics::vulkan
