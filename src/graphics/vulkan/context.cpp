#include "context.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VulkanContext::VulkanContext(VulkanObjects&& objects)
        : objects_(std::move(objects))
    {
        ND_SET_SCOPE();
    }

    VulkanContext::~VulkanContext()
    {
        ND_SET_SCOPE();

        vkDeviceWaitIdle(objects_.device_.handle);

        for(size_t index = 0; index < objects_.framesCount_; ++index)
        {
            vkDestroySemaphore(objects_.device_.handle, objects_.imageAcquiredSemaphores_[index], nullptr);
            vkDestroySemaphore(objects_.device_.handle, objects_.imageRenderedSemaphores_[index], nullptr);

            const auto fences = std::vector<VkFence> {objects_.imageAcquiredFences_[index], objects_.imageRenderedFences_[index]};

            vkWaitForFences(objects_.device_.handle, fences.size(), fences.data(), VK_TRUE, UINT64_MAX);

            vkDestroyFence(objects_.device_.handle, objects_.imageAcquiredFences_[index], nullptr);
            vkDestroyFence(objects_.device_.handle, objects_.imageRenderedFences_[index], nullptr);
        }

        for(size_t bufferIndex = 0; bufferIndex < objects_.buffers_.size(); ++bufferIndex)
        {
            for(size_t memoryIndex = 0; memoryIndex < objects_.bufferMemories_[bufferIndex].size(); ++memoryIndex)
            {
                vkFreeMemory(objects_.device_.handle, objects_.bufferMemories_[bufferIndex][memoryIndex].handle, nullptr);
            }

            vkDestroyBuffer(objects_.device_.handle, objects_.buffers_[bufferIndex], nullptr);
        }

        for(size_t index = 0; index < objects_.commandPools_.size(); ++index)
        {
            vkFreeCommandBuffers(objects_.device_.handle,
                                 objects_.commandPools_[index].handle,
                                 objects_.commandBuffers_[index].size(),
                                 objects_.commandBuffers_[index].data());

            vkDestroyCommandPool(objects_.device_.handle, objects_.commandPools_[index].handle, nullptr);
        }

        for(const auto graphicsPipeline: objects_.graphicsPipelines_)
        {
            vkDestroyPipeline(objects_.device_.handle, graphicsPipeline, nullptr);
        }

        for(const auto pipelineLayout: objects_.pipelineLayouts_)
        {
            vkDestroyPipelineLayout(objects_.device_.handle, pipelineLayout, nullptr);
        }

        vkFreeDescriptorSets(objects_.device_.handle, objects_.descriptorPool_, objects_.descriptorSets_.size(), objects_.descriptorSets_.data());

        for(const auto descriptorSetLayout: objects_.descriptorSetLayouts_)
        {
            vkDestroyDescriptorSetLayout(objects_.device_.handle, descriptorSetLayout, nullptr);
        }

        vkDestroyDescriptorPool(objects_.device_.handle, objects_.descriptorPool_, nullptr);

        for(const auto shaderModule: objects_.shaderModules_)
        {
            vkDestroyShaderModule(objects_.device_.handle, shaderModule.handle, nullptr);
        }

        for(const auto swapchainFramebuffer: objects_.swapchainFramebuffers_)
        {
            vkDestroyFramebuffer(objects_.device_.handle, swapchainFramebuffer, nullptr);
        }

        for(const auto swapchainImageView: objects_.swapchainImageViews_)
        {
            vkDestroyImageView(objects_.device_.handle, swapchainImageView, nullptr);
        }

        vkDestroyRenderPass(objects_.device_.handle, objects_.renderPass_, nullptr);
        vkDestroySwapchainKHR(objects_.device_.handle, objects_.swapchain_.handle, nullptr);
        vkDestroySurfaceKHR(objects_.instance_, objects_.surface_, nullptr);
        vkDestroyDevice(objects_.device_.handle, nullptr);
        vkDestroyInstance(objects_.instance_, nullptr);
    }

    void
    VulkanContext::drawNextFrame()
    {
        ND_SET_SCOPE();

        static auto frameIndex = size_t {0};

        static auto deviceQueue    = objects_.device_.queues.begin()->second[0];
        static auto swapchainQueue = objects_.swapchain_.queues.begin()->second[0];

        const auto imageAcquiredSemaphore = objects_.imageAcquiredSemaphores_[frameIndex];
        const auto imageRenderedSemaphore = objects_.imageRenderedSemaphores_[frameIndex];
        const auto imageAcquiredFence     = objects_.imageAcquiredFences_[frameIndex];
        const auto imageRenderedFence     = objects_.imageRenderedFences_[frameIndex];

        const auto fences = std::vector<VkFence> {imageAcquiredFence, imageRenderedFence};

        vkWaitForFences(objects_.device_.handle, fences.size(), fences.data(), VK_TRUE, UINT64_MAX);
        vkResetFences(objects_.device_.handle, fences.size(), fences.data());

        const auto imageIndex = getNextSwapchainImage(objects_.device_.handle,
                                                      objects_.swapchain_.handle,
                                                      imageAcquiredSemaphore,
                                                      imageAcquiredFence);

        const auto commandBuffers   = std::vector<VkCommandBuffer> {objects_.commandBuffers_[0][imageIndex]};
        const auto waitDstStageMask = std::vector<VkPipelineStageFlags> {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        const auto waitSemaphores   = std::vector<VkSemaphore> {imageAcquiredSemaphore};
        const auto signalSemaphores = std::vector<VkSemaphore> {imageRenderedSemaphore};

        const auto submitInfos = std::vector<VkSubmitInfo> {getSubmitInfo({commandBuffers, waitDstStageMask, waitSemaphores, signalSemaphores})};

        ND_ASSERT_EXEC(vkQueueSubmit(deviceQueue, submitInfos.size(), submitInfos.data(), imageRenderedFence) == VK_SUCCESS);

        const auto swapchains   = std::vector<VkSwapchainKHR> {objects_.swapchain_.handle};
        const auto imageIndices = std::vector<uint32_t> {static_cast<uint32_t>(imageIndex)};

        const auto presentInfo = getPresentInfo({swapchains, signalSemaphores, imageIndices});

        ND_ASSERT_EXEC(vkQueuePresentKHR(swapchainQueue, &presentInfo) == VK_SUCCESS);

        frameIndex = (frameIndex + 1) % objects_.framesCount_;
    }

    VulkanContext
    getVulkanContext(const VulkanContextConfigurationExternal& configurationExternal,
                     const VulkanContextInitializers&          initializers,
                     const VulkanContextConfigurations&        configurations)
    {
        ND_SET_SCOPE();

        const auto width  = configurationExternal.width;
        const auto height = configurationExternal.height;

        const auto instanceConfig = configurations.getInstance(configurationExternal);
        const auto instance       = initializers.getInstance(instanceConfig);

        const auto physicalDeviceConfig = configurations.getPhysicalDevice();
        const auto physicalDevice       = initializers.getPhysicalDevice(physicalDeviceConfig, instance);

        const auto physicalDeviceMemoryProperties = getPhysicalDeviceMemoryProperties(physicalDevice);

        const auto deviceConfig = configurations.getDevice(physicalDeviceConfig);
        const auto device       = initializers.getDevice(deviceConfig, physicalDevice);

        const auto surface = initializers.getSurface(instance);

        const auto swapchainConfig = configurations.getSwapchain(physicalDevice, surface, width, height);
        const auto swapchain       = initializers.getSwapchain(swapchainConfig, device.handle);

        const auto renderPassConfig = configurations.getRenderPass(swapchainConfig);
        const auto renderPass       = initializers.getRenderPass(renderPassConfig, device.handle);

        const auto swapchainImages = initializers.getSwapchainImages(swapchain.handle, device.handle);

        const auto swapchainImageViewConfigs = configurations.getSwapchainImageViews(swapchainConfig, swapchainImages);
        const auto swapchainImageViews       = initializers.getSwapchainImageViews(swapchainImageViewConfigs, device.handle);

        const auto swapchainFramebufferConfigs = configurations.getSwapchainFramebuffers(swapchainConfig, swapchainImageViews, renderPass);
        const auto swapchainFramebuffers       = initializers.getSwapchainFramebuffers(swapchainFramebufferConfigs, device.handle);

        const auto shaderModuleConfigs = configurations.getShaderModules();
        const auto shaderModules       = initializers.getShaderModules(shaderModuleConfigs, device.handle);

        const auto descriptorPoolConfig = configurations.getDescriptorPool();
        const auto descriptorPool       = initializers.getDescriptorPool(descriptorPoolConfig, device.handle);

        const auto descriptorSetLayoutConfigs = configurations.getDescriptorSetLayouts();
        const auto descriptorSetLayouts       = initializers.getDescriptorSetLayouts(descriptorSetLayoutConfigs, device.handle);

        const auto descriptorSetConfig = configurations.getDescriptorSet(descriptorSetLayouts, descriptorPool);
        const auto descriptorSets      = initializers.getDescriptorSets(descriptorSetConfig, device.handle);

        const auto pipelineLayoutConfigs = configurations.getPipelineLayouts(descriptorSetLayouts);
        const auto pipelineLayouts       = initializers.getPipelineLayouts(pipelineLayoutConfigs, device.handle);

        const auto graphicsPipelineConfigs = configurations.getGraphicsPipelines(shaderModules, pipelineLayouts, renderPass, width, height);
        const auto graphicsPipelines       = initializers.getGraphicsPipelines(graphicsPipelineConfigs, device.handle);

        const auto commandPoolConfigs = configurations.getCommandPools(device.queueFamilies);
        const auto commandPools       = initializers.getCommandPools(commandPoolConfigs, device.handle);

        const auto commandBufferConfigs = configurations.getCommandBuffers(commandPools);
        const auto commandBuffers       = initializers.getCommandBuffers(commandBufferConfigs, device.handle);

        const auto bufferConfigs = configurations.getBuffers(device.queueFamilies);
        const auto buffers       = initializers.getBuffers(bufferConfigs, device.handle);

        const auto bufferMemoryConfigs = getMapped<Buffer, std::vector<DeviceMemoryConfiguration>>(
            buffers,
            [&configurations, &physicalDeviceMemoryProperties, device = device.handle](const auto& buffer, const auto index)
            {
                const auto memoryRequirements = getMemoryRequirements(device, buffer);

                return configurations.getBufferMemories(physicalDeviceMemoryProperties, memoryRequirements);
            });

        const auto bufferMemories = getMapped<std::vector<DeviceMemoryConfiguration>, std::vector<DeviceMemory>>(
            bufferMemoryConfigs,
            [&initializers, device = device.handle](const auto& bufferMemoryConfig, const auto index)
            {
                return initializers.getBufferMemories(bufferMemoryConfig, device);
            });

        vkBindBufferMemory(device.handle, buffers[0], bufferMemories[0][0].handle, 0);

        const auto vertices = std::vector<Vertex> {{{0.0, -0.5, 0.0}, {1.0, 0.0, 0.0}},
                                                   {{0.5, 0.5, 0.0}, {0.0, 1.0, 0.0}},
                                                   {{-0.5, 0.5, 0.0}, {0.0, 0.0, 1.0}}};

        setMemory(device.handle, bufferMemories[0][0], 0, vertices.data());

        for(size_t i = 0; i < swapchainFramebuffers.size(); ++i)
        {
            const auto framebuffer   = swapchainFramebuffers[i];
            const auto commandBuffer = commandBuffers[0][i];

            const auto commandBufferBeginInfo = getCommandBufferBeginInfo(nullptr);

            ND_ASSERT_EXEC(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) == VK_SUCCESS);

            const auto clearValues         = std::vector<VkClearValue> {{{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f}}};
            const auto renderPassBeginInfo = getRenderPassBeginInfo(renderPass,
                                                                    framebuffer,
                                                                    {{0u, 0u}, {width, height}},
                                                                    clearValues.size(),
                                                                    clearValues.data());

            vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelines[0]);

            const auto vertexBuffers        = std::vector<VkBuffer> {buffers[0]};
            const auto vertexBuffersOffsets = std::vector<VkDeviceSize> {0};

            vkCmdBindVertexBuffers(commandBuffer, 0, vertexBuffers.size(), vertexBuffers.data(), vertexBuffersOffsets.data());
            vkCmdDraw(commandBuffer, 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffer);

            ND_ASSERT_EXEC(vkEndCommandBuffer(commandBuffer) == VK_SUCCESS);
        }

        const auto framesCount = size_t {swapchainImages.size()};

        const auto imageAcquiredSemaphores = getSemaphore(device.handle, framesCount);
        const auto imageRenderedSemaphores = getSemaphore(device.handle, framesCount);
        const auto imageAcquiredFences     = getFence(device.handle, framesCount, VK_FENCE_CREATE_SIGNALED_BIT);
        const auto imageRenderedFences     = getFence(device.handle, framesCount, VK_FENCE_CREATE_SIGNALED_BIT);

        return VulkanContext({physicalDeviceMemoryProperties,
                              device,
                              swapchain,
                              swapchainImages,
                              swapchainImageViews,
                              swapchainFramebuffers,
                              shaderModules,
                              descriptorSetLayouts,
                              descriptorSets,
                              pipelineLayouts,
                              graphicsPipelines,
                              commandPools,
                              commandBuffers,
                              buffers,
                              bufferMemories,
                              imageAcquiredSemaphores,
                              imageRenderedSemaphores,
                              imageAcquiredFences,
                              imageRenderedFences,
                              framesCount,
                              instance,
                              physicalDevice,
                              surface,
                              renderPass,
                              descriptorPool});
    }
} // namespace nd::src::graphics::vulkan
