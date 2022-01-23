#pragma once

#include "pch.hpp"

#include "context_configurations.hpp"
#include "context_initializers.hpp"

namespace nd::src::graphics::vulkan
{
    class VulkanObjects final
    {
    public:
        Device    device {};
        Swapchain swapchain {};

        std::vector<DeviceMemory>        deviceMemories {};
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

        std::vector<size_t> bufferMemoryIndices {};
        std::vector<size_t> imageMemoryIndices {};

        std::vector<VkSemaphore> semaphores {};
        std::vector<VkFence>     fences {};

        PhysicalDevice physicalDevice {};
        Instance       instance {};
        Surface        surface {};
        RenderPass     renderPass {};
        DescriptorPool descriptorPool {};

        VulkanObjects(Instance                           instance,
                      Surface                            surface,
                      RenderPass                         renderPass,
                      DescriptorPool                     descriptorPool,
                      PhysicalDevice&&                   physicalDevice,
                      Device&&                           device,
                      Swapchain&&                        swapchain,
                      std::vector<DeviceMemory>&&        deviceMemories,
                      std::vector<Image>&&               swapchainImages,
                      std::vector<ImageView>&&           swapchainImageViews,
                      std::vector<Framebuffer>&&         swapchainFramebuffers,
                      std::vector<ShaderModule>&&        shaderModules,
                      std::vector<DescriptorSetLayout>&& descriptorSetLayouts,
                      std::vector<DescriptorSet>&&       descriptorSets,
                      std::vector<PipelineLayout>&&      pipelineLayouts,
                      std::vector<Pipeline>&&            graphicsPipelines,
                      std::vector<CommandPool>&&         commandPools,
                      std::vector<CommandBuffers>&&      commandBuffers,
                      std::vector<Buffer>&&              buffers,
                      std::vector<size_t>&&              bufferMemoryIndices,
                      std::vector<size_t>&&              imageMemoryIndices);

        VulkanObjects(const VulkanObjects& vulkanObjects) = delete;
        VulkanObjects(VulkanObjects&& vulkanObjects) noexcept;

        VulkanObjects&
        operator=(const VulkanObjects& vulkanObjects) = delete;
        VulkanObjects&
        operator=(VulkanObjects&& vulkanObjects) noexcept;

        ~VulkanObjects();
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

        const QueueFamily&
        getGraphicsQueueFamily() const;

        const QueueFamily&
        getComputeQueueFamily() const;

        const QueueFamily&
        getTransferQueueFamily() const;

        const QueueFamily&
        getSwapchainQueueFamily() const;

        const CommandBuffers&
        getGraphicsCommandBuffers() const;

        const CommandBuffers&
        getTransferCommandBuffers() const;

        constexpr int
        getVertexBufferIndex() const;

        constexpr int
        getIndexBufferIndex() const;

        constexpr int
        getStagingBufferIndex() const;

        const Buffer&
        getVertexBuffer() const;

        const Buffer&
        getIndexBuffer() const;

        const Buffer&
        getStagingBuffer() const;

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
