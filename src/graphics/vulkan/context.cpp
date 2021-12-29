#include "context.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VulkanContext::VulkanContext(const Configuration& configuration)
        : instance_(configuration.instance)
        , device_(configuration.device)
        , surface_(configuration.surface)
        , swapchain_(configuration.swapchain)
        , renderPass_(configuration.renderPass)
        , swapchainImages_(configuration.swapchainImages)
        , swapchainImageViews_(configuration.swapchainImageViews)
        , swapchainFramebuffers_(configuration.swapchainFramebuffers)
        , shaderModules_(configuration.shaderModules)
        , descriptorPool_(configuration.descriptorPool)
        , descriptorSetLayouts_(configuration.descriptorSetLayouts)
        , descriptorSet_(configuration.descriptorSet)
        , pipelineLayouts_(configuration.pipelineLayouts)
        , graphicsPipelines_(configuration.graphicsPipelines)
        , commandPools_(configuration.commandPools)
        , commandBuffers_(configuration.commandBuffers)
        , framesCount_(configuration.framesCount)
        , imageAcquiredSemaphores_(configuration.imageAcquiredSemaphores)
        , imageRenderedSemaphores_(configuration.imageRenderedSemaphores)
        , imageAcquiredFences_(configuration.imageAcquiredFences)
        , imageRenderedFences_(configuration.imageRenderedFences)
    {
        ND_SET_SCOPE();
    }

    VulkanContext::~VulkanContext()
    {
        ND_SET_SCOPE();

        vkDeviceWaitIdle(device_.handle);

        for(size_t index = 0; index < framesCount_; ++index)
        {
            vkDestroySemaphore(device_.handle, imageAcquiredSemaphores_[index], nullptr);
            vkDestroySemaphore(device_.handle, imageRenderedSemaphores_[index], nullptr);

            const auto fences = std::vector<VkFence> {imageAcquiredFences_[index], imageRenderedFences_[index]};

            vkWaitForFences(device_.handle, fences.size(), fences.data(), VK_TRUE, UINT64_MAX);

            vkDestroyFence(device_.handle, imageAcquiredFences_[index], nullptr);
            vkDestroyFence(device_.handle, imageRenderedFences_[index], nullptr);
        }

        for(size_t index = 0; index < commandPools_.size(); ++index)
        {
            vkFreeCommandBuffers(device_.handle,
                                 commandPools_[index].handle,
                                 commandBuffers_[index].handles.size(),
                                 commandBuffers_[index].handles.data());

            vkDestroyCommandPool(device_.handle, commandPools_[index].handle, nullptr);
        }

        for(const auto pipeline: graphicsPipelines_.handles)
        {
            vkDestroyPipeline(device_.handle, pipeline, nullptr);
        }

        for(const auto pipelineLayout: pipelineLayouts_)
        {
            vkDestroyPipelineLayout(device_.handle, pipelineLayout.handle, nullptr);
        }

        vkFreeDescriptorSets(device_.handle, descriptorPool_.handle, descriptorSet_.handles.size(), descriptorSet_.handles.data());

        for(const auto descriptorSetLayout: descriptorSetLayouts_)
        {
            vkDestroyDescriptorSetLayout(device_.handle, descriptorSetLayout.handle, nullptr);
        }

        vkDestroyDescriptorPool(device_.handle, descriptorPool_.handle, nullptr);

        for(const auto shaderModule: shaderModules_)
        {
            vkDestroyShaderModule(device_.handle, shaderModule.handle, nullptr);
        }

        for(const auto swapchainFramebuffer: swapchainFramebuffers_)
        {
            vkDestroyFramebuffer(device_.handle, swapchainFramebuffer.handle, nullptr);
        }

        for(const auto swapchainImageView: swapchainImageViews_)
        {
            vkDestroyImageView(device_.handle, swapchainImageView.handle, nullptr);
        }

        vkDestroyRenderPass(device_.handle, renderPass_.handle, nullptr);
        vkDestroySwapchainKHR(device_.handle, swapchain_.handle, nullptr);
        vkDestroySurfaceKHR(instance_.handle, surface_.handle, nullptr);
        vkDestroyDevice(device_.handle, nullptr);
        vkDestroyInstance(instance_.handle, nullptr);
    }

    void
    VulkanContext::drawNextFrame()
    {
        ND_SET_SCOPE();

        static auto frameIndex = size_t {0};

        static auto deviceQueue    = device_.queues.begin()->second[0];
        static auto swapchainQueue = swapchain_.queues.begin()->second[0];

        const auto imageAcquiredSemaphore = imageAcquiredSemaphores_[frameIndex];
        const auto imageRenderedSemaphore = imageRenderedSemaphores_[frameIndex];
        const auto imageAcquiredFence     = imageAcquiredFences_[frameIndex];
        const auto imageRenderedFence     = imageRenderedFences_[frameIndex];

        const auto fences = std::vector<VkFence> {imageAcquiredFence, imageRenderedFence};

        vkWaitForFences(device_.handle, fences.size(), fences.data(), VK_TRUE, UINT64_MAX);
        vkResetFences(device_.handle, fences.size(), fences.data());

        const auto imageIndex = getNextSwapchainImage(device_.handle, swapchain_.handle, imageAcquiredSemaphore, imageAcquiredFence);

        const auto commandBuffers   = std::vector<VkCommandBuffer> {commandBuffers_[0].handles[imageIndex]};
        const auto waitDstStageMask = std::vector<VkPipelineStageFlags> {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        const auto waitSemaphores   = std::vector<VkSemaphore> {imageAcquiredSemaphore};
        const auto signalSemaphores = std::vector<VkSemaphore> {imageRenderedSemaphore};

        const auto submitInfos = std::vector<VkSubmitInfo> {getSubmitInfo({commandBuffers, waitDstStageMask, waitSemaphores, signalSemaphores})};

        ND_ASSERT_EXEC(vkQueueSubmit(deviceQueue, submitInfos.size(), submitInfos.data(), imageRenderedFence) == VK_SUCCESS);

        const auto swapchains   = std::vector<VkSwapchainKHR> {swapchain_.handle};
        const auto imageIndices = std::vector<uint32_t> {static_cast<uint32_t>(imageIndex)};

        const auto presentInfo = getPresentInfo({swapchains, signalSemaphores, imageIndices});

        ND_ASSERT_EXEC(vkQueuePresentKHR(swapchainQueue, &presentInfo) == VK_SUCCESS);

        frameIndex = (frameIndex + 1) % framesCount_;
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
        const auto physicalDevice       = initializers.getPhysicalDevice(physicalDeviceConfig, instance.handle);

        const auto deviceConfig = configurations.getDevice(physicalDeviceConfig);
        const auto device       = initializers.getDevice(deviceConfig, physicalDevice.handle);

        const auto surface = initializers.getSurface(instance.handle);

        const auto swapchainConfig = configurations.getSwapchain(physicalDevice.handle, surface.handle, width, height);
        const auto swapchain       = initializers.getSwapchain(swapchainConfig, device.handle);

        const auto renderPassConfig = configurations.getRenderPass(swapchainConfig);
        const auto renderPass       = initializers.getRenderPass(renderPassConfig, device.handle);

        const auto swapchainImages = initializers.getSwapchainImages(swapchain.handle, device.handle);

        const auto swapchainImageViewConfig = configurations.getSwapchainImageViews(swapchainConfig, swapchainImages);
        const auto swapchainImageViews      = initializers.getSwapchainImageViews(swapchainImageViewConfig, device.handle);

        const auto swapchainFramebufferConfig = configurations.getSwapchainFramebuffers(swapchainConfig, swapchainImageViews, renderPass.handle);
        const auto swapchainFramebuffers      = initializers.getSwapchainFramebuffers(swapchainFramebufferConfig, device.handle);

        const auto shaderModuleConfig = configurations.getShaderModules();
        const auto shaderModules      = initializers.getShaderModules(shaderModuleConfig, device.handle);

        const auto descriptorPoolConfig = configurations.getDescriptorPool();
        const auto descriptorPool       = initializers.getDescriptorPool(descriptorPoolConfig, device.handle);

        const auto descriptorSetLayoutConfig = configurations.getDescriptorSetLayouts();
        const auto descriptorSetLayouts      = initializers.getDescriptorSetLayouts(descriptorSetLayoutConfig, device.handle);

        const auto descriptorSetsConfig = configurations.getDescriptorSet(descriptorSetLayouts, descriptorPool.handle);
        const auto descriptorSets       = initializers.getDescriptorSet(descriptorSetsConfig, device.handle);

        const auto pipelineLayoutConfig = configurations.getPipelineLayouts(descriptorSetLayouts);
        const auto pipelineLayouts      = initializers.getPipelineLayouts(pipelineLayoutConfig, device.handle);

        const auto graphicsPipelineConfig = configurations.getGraphicsPipelines(shaderModules, pipelineLayouts, renderPass.handle, width, height);
        const auto graphicsPipelines      = initializers.getGraphicsPipelines(graphicsPipelineConfig, device.handle);

        const auto commandPoolConfig = configurations.getCommandPools(device.queueFamilies);
        const auto commandPools      = initializers.getCommandPools(commandPoolConfig, device.handle);

        const auto commandBufferConfig = configurations.getCommandBuffers(commandPools);
        const auto commandBuffers      = initializers.getCommandBuffers(commandBufferConfig, device.handle);

        for(size_t i = 0; i < swapchainFramebuffers.size(); ++i)
        {
            const auto framebuffer   = swapchainFramebuffers[i];
            const auto commandBuffer = commandBuffers[0].handles[i];

            const auto commandBufferBeginInfo = getCommandBufferBeginInfo(nullptr);

            ND_ASSERT_EXEC(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) == VK_SUCCESS);

            const auto clearValues         = std::vector<VkClearValue> {{{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f}}};
            const auto renderPassBeginInfo = getRenderPassBeginInfo(renderPass.handle,
                                                                    framebuffer.handle,
                                                                    {{0u, 0u}, {width, height}},
                                                                    clearValues.size(),
                                                                    clearValues.data());

            vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelines.handles[0]);
            vkCmdDraw(commandBuffer, 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffer);

            ND_ASSERT_EXEC(vkEndCommandBuffer(commandBuffer) == VK_SUCCESS);
        }

        const auto framesCount = size_t {2};

        const auto imageAcquiredSemaphores = getSemaphore(device.handle, framesCount);
        const auto imageRenderedSemaphores = getSemaphore(device.handle, framesCount);
        const auto imageAcquiredFences     = getFence(device.handle, framesCount, VK_FENCE_CREATE_SIGNALED_BIT);
        const auto imageRenderedFences     = getFence(device.handle, framesCount, VK_FENCE_CREATE_SIGNALED_BIT);

        return VulkanContext({device,
                              swapchain,
                              swapchainImages,
                              swapchainImageViews,
                              swapchainFramebuffers,
                              shaderModules,
                              descriptorSetLayouts,
                              pipelineLayouts,
                              commandPools,
                              commandBuffers,
                              graphicsPipelines,
                              descriptorSets,
                              imageAcquiredSemaphores,
                              imageRenderedSemaphores,
                              imageAcquiredFences,
                              imageRenderedFences,
                              framesCount,
                              instance,
                              surface,
                              renderPass,
                              descriptorPool});
    }
} // namespace nd::src::graphics::vulkan
