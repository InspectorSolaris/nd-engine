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
        , descriptorSetLayout_(configuration.descriptorSetLayout)
        , descriptorSets_(configuration.descriptorSets)
        , pipelineLayout_(configuration.pipelineLayout)
        , pipelines_(configuration.pipelines)
        , commandPool_(configuration.commandPool)
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

        vkFreeCommandBuffers(device_.handle, commandPool_.handle, commandBuffers_.handles.size(), commandBuffers_.handles.data());

        vkDestroyCommandPool(device_.handle, commandPool_.handle, nullptr);

        for(const auto pipeline: pipelines_.handles)
        {
            vkDestroyPipeline(device_.handle, pipeline, nullptr);
        }

        vkDestroyPipelineLayout(device_.handle, pipelineLayout_.handle, nullptr);

        vkFreeDescriptorSets(device_.handle, descriptorPool_.handle, descriptorSets_.handles.size(), descriptorSets_.handles.data());

        vkDestroyDescriptorSetLayout(device_.handle, descriptorSetLayout_.handle, nullptr);
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

        const auto commandBuffers   = std::vector<VkCommandBuffer> {commandBuffers_.handles[imageIndex]};
        const auto waitDstStageMask = std::vector<VkPipelineStageFlags> {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        const auto waitSemaphores   = std::vector<VkSemaphore> {imageAcquiredSemaphore};
        const auto signalSemaphores = std::vector<VkSemaphore> {imageRenderedSemaphore};

        const auto submitInfo = getSubmitInfo({commandBuffers, waitDstStageMask, waitSemaphores, signalSemaphores});

        ND_ASSERT_EXEC(vkQueueSubmit(deviceQueue, 1, &submitInfo, imageRenderedFence) == VK_SUCCESS);

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

        const auto instanceConfiguration = configurations.getInstance(configurationExternal);
        const auto instance              = initializers.getInstance(instanceConfiguration);

        const auto physicalDeviceConfiguration = configurations.getPhysicalDevice();
        const auto physicalDevice              = initializers.getPhysicalDevice(physicalDeviceConfiguration, instance.handle);

        const auto deviceConfiguration = configurations.getDevice(physicalDeviceConfiguration);
        const auto device              = initializers.getDevice(deviceConfiguration, physicalDevice.handle);

        const auto surface = initializers.getSurface(instance.handle);

        const auto swapchainConfiguration = configurations.getSwapchain(physicalDevice.handle,
                                                                        surface.handle,
                                                                        configurationExternal.width,
                                                                        configurationExternal.height);

        const auto swapchain = initializers.getSwapchain(swapchainConfiguration, device.handle);

        const auto renderPassConfiguration = configurations.getRenderPass(swapchainConfiguration);
        const auto renderPass              = initializers.getRenderPass(renderPassConfiguration, device.handle);

        const auto swapchainImages = initializers.getSwapchainImages(swapchain.handle, device.handle);

        const auto swapchainImageViewConfigurations = configurations.getSwapchainImageViews(swapchainConfiguration, swapchainImages);
        const auto swapchainImageViews              = initializers.getSwapchainImageViews(swapchainImageViewConfigurations, device.handle);

        const auto swapchainFramebufferConfigurations = configurations.getSwapchainFramebuffers(swapchainConfiguration,
                                                                                                swapchainImageViews,
                                                                                                renderPass.handle);

        const auto swapchainFramebuffers = initializers.getSwapchainFramebuffers(swapchainFramebufferConfigurations, device.handle);

        const auto shaderModulesConfigurations = std::vector<ShaderModuleConfiguration> {
            {"src/graphics/vulkan/shaders/vert.spv", VK_SHADER_STAGE_VERTEX_BIT},
            {"src/graphics/vulkan/shaders/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT}};

        const auto shaderModules = getMapped<ShaderModuleConfiguration, ShaderModule>(
            shaderModulesConfigurations,
            [device = device.handle](const auto& shaderModuleConfiguration, const auto index)
            {
                return getShaderModule(shaderModuleConfiguration, device);
            });

        const auto descriptorPool = getDescriptorPool(
            {{{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1}}, 1, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT},
            device.handle);

        const auto descriptorSetLayout = getDescriptorSetLayout(DescriptorSetLayoutConfiguration {{}}, device.handle);
        const auto descriptorSets      = getDescriptorSets({{descriptorSetLayout.handle}, descriptorPool.handle}, device.handle);

        const auto pipelineLayout = getPipelineLayout({{descriptorSetLayout.handle}, {}}, device.handle);

        const auto vertexInputStateCreateInfo   = getPipelineVertexInputStateCreateInfo(0, 0, nullptr, nullptr);
        const auto inputAssemblyStateCreateInfo = getPipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

        const auto tessellationStateCreateInfo = VkPipelineTessellationStateCreateInfo {};

        const auto scissors = VkRect2D {{0, 0}, {swapchainConfiguration.imageExtent.width, swapchainConfiguration.imageExtent.height}};
        const auto viewport = VkViewport {0.0f,
                                          0.0f,
                                          static_cast<float>(swapchainConfiguration.imageExtent.width),
                                          static_cast<float>(swapchainConfiguration.imageExtent.height),
                                          0.0f,
                                          0.1f};

        const auto viewportStateCreateInfo = getPipelineViewportStateCreateInfo(1, 1, &viewport, &scissors);

        const auto rasterizationStateCreateInfo = getPipelineRasterizationStateCreateInfo(VK_FALSE,
                                                                                          VK_FALSE,
                                                                                          VK_POLYGON_MODE_FILL,
                                                                                          VK_CULL_MODE_NONE,
                                                                                          VK_FRONT_FACE_CLOCKWISE,
                                                                                          VK_FALSE,
                                                                                          0.0f,
                                                                                          0.0f,
                                                                                          0.0f,
                                                                                          1.0f);

        const auto multisampleStateCreateInfo = getPipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT,
                                                                                      VK_FALSE,
                                                                                      1.0f,
                                                                                      nullptr,
                                                                                      VK_FALSE,
                                                                                      VK_FALSE);

        const auto depthStencilStateCreateInfo = VkPipelineDepthStencilStateCreateInfo {};

        const auto blendConstants       = std::vector<float> {0.0f, 0.0f, 0.0f, 0.0f};
        const auto colorBlendAttachment = VkPipelineColorBlendAttachmentState {
            VK_TRUE,
            VK_BLEND_FACTOR_SRC_ALPHA,
            VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            VK_BLEND_OP_ADD,
            VK_BLEND_FACTOR_ONE,
            VK_BLEND_FACTOR_ZERO,
            VK_BLEND_OP_ADD,
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};

        const auto colorBlendStateCreateInfo = getPipelineColorBlendStateCreateInfo(VK_FALSE,
                                                                                    VK_LOGIC_OP_OR,
                                                                                    1,
                                                                                    &colorBlendAttachment,
                                                                                    blendConstants.data());

        const auto dynamicStateCreateInfo = getPipelineDynamicStateCreateInfo(0, nullptr);

        const auto pipelines = getGraphicsPipelines(
            {{getMapped<VkShaderModule, VkPipelineShaderStageCreateInfo>(
                  shaderModules,
                  [](const auto& shaderModule, const auto index)
                  {
                      return getPipelineShaderStageCreateInfo(shaderModule.stage, shaderModule.handle, "main", nullptr);
                  }),
              &vertexInputStateCreateInfo,
              &inputAssemblyStateCreateInfo,
              nullptr,
              &viewportStateCreateInfo,
              &rasterizationStateCreateInfo,
              &multisampleStateCreateInfo,
              nullptr,
              &colorBlendStateCreateInfo,
              &dynamicStateCreateInfo,
              pipelineLayout.handle,
              renderPass.handle,
              0}},
            device.handle);

        const auto graphicsQueueFamily = std::find_if(device.queueFamilies.begin(),
                                                      device.queueFamilies.end(),
                                                      [](const auto& queueFamily)
                                                      {
                                                          return isSubmask(queueFamily.queueFlags, VK_QUEUE_GRAPHICS_BIT);
                                                      });

        ND_ASSERT(graphicsQueueFamily != device.queueFamilies.end());

        const auto commandPool    = getCommandPool({graphicsQueueFamily->index}, device.handle);
        const auto commandBuffers = getCommandBuffers(
            {commandPool.handle, VK_COMMAND_BUFFER_LEVEL_PRIMARY, static_cast<uint32_t>(swapchainFramebuffers.size())},
            device.handle);

        for(size_t i = 0; i < commandBuffers.handles.size(); ++i)
        {
            const auto framebuffer   = swapchainFramebuffers[i];
            const auto commandBuffer = commandBuffers.handles[i];

            const auto commandBufferBeginInfo = getCommandBufferBeginInfo(nullptr);

            ND_ASSERT_EXEC(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) == VK_SUCCESS);

            const auto clearValues         = std::vector<VkClearValue> {{{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f}}};
            const auto renderPassBeginInfo = getRenderPassBeginInfo(renderPass.handle,
                                                                    framebuffer.handle,
                                                                    scissors,
                                                                    clearValues.size(),
                                                                    clearValues.data());

            vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.handles[0]);
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
                              pipelines,
                              descriptorSets,
                              commandBuffers,
                              imageAcquiredSemaphores,
                              imageRenderedSemaphores,
                              imageAcquiredFences,
                              imageRenderedFences,
                              framesCount,
                              instance,
                              surface,
                              renderPass,
                              descriptorPool,
                              descriptorSetLayout,
                              pipelineLayout,
                              commandPool});
    }
} // namespace nd::src::graphics::vulkan
