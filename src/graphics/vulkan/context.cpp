#include "context.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VulkanContext::VulkanContext(const Configuration& configuration)
        : instance_(configuration.instance)
        , device_(configuration.device)
        , deviceQueueFamilies_(configuration.deviceQueueFamilies)
        , deviceQueues_(configuration.deviceQueues)
        , surface_(configuration.surface)
        , swapchain_(configuration.swapchain)
        , swapchainQueueFamilies_(configuration.swapchainQueueFamilies)
        , swapchainQueues_(configuration.swapchainQueues)
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

        vkDeviceWaitIdle(device_);

        for(size_t index = 0; index < framesCount_; ++index)
        {
            vkDestroySemaphore(device_, imageAcquiredSemaphores_[index], nullptr);
            vkDestroySemaphore(device_, imageRenderedSemaphores_[index], nullptr);

            const auto fences = std::vector<VkFence> {imageAcquiredFences_[index], imageRenderedFences_[index]};

            vkWaitForFences(device_, fences.size(), fences.data(), VK_TRUE, UINT64_MAX);

            vkDestroyFence(device_, imageAcquiredFences_[index], nullptr);
            vkDestroyFence(device_, imageRenderedFences_[index], nullptr);
        }

        vkFreeCommandBuffers(device_, commandPool_, commandBuffers_.size(), commandBuffers_.data());
        vkDestroyCommandPool(device_, commandPool_, nullptr);

        for(const auto pipeline: pipelines_)
        {
            vkDestroyPipeline(device_, pipeline, nullptr);
        }

        vkDestroyPipelineLayout(device_, pipelineLayout_, nullptr);

        vkFreeDescriptorSets(device_, descriptorPool_, descriptorSets_.size(), descriptorSets_.data());

        vkDestroyDescriptorSetLayout(device_, descriptorSetLayout_, nullptr);
        vkDestroyDescriptorPool(device_, descriptorPool_, nullptr);

        for(const auto shaderModule: shaderModules_)
        {
            vkDestroyShaderModule(device_, shaderModule, nullptr);
        }

        for(const auto swapchainFramebuffer: swapchainFramebuffers_)
        {
            vkDestroyFramebuffer(device_, swapchainFramebuffer, nullptr);
        }

        for(const auto swapchainImageView: swapchainImageViews_)
        {
            vkDestroyImageView(device_, swapchainImageView, nullptr);
        }

        vkDestroyRenderPass(device_, renderPass_, nullptr);
        vkDestroySwapchainKHR(device_, swapchain_, nullptr);
        vkDestroySurfaceKHR(instance_, surface_, nullptr);
        vkDestroyDevice(device_, nullptr);
        vkDestroyInstance(instance_, nullptr);
    }

    void
    VulkanContext::drawNextFrame()
    {
        ND_SET_SCOPE();

        static auto frameIndex = size_t {0};

        const auto deviceQueue    = deviceQueues_.begin()->second[0];
        const auto swapchainQueue = swapchainQueues_.begin()->second[0];

        const auto imageAcquiredSemaphore = imageAcquiredSemaphores_[frameIndex];
        const auto imageRenderedSemaphore = imageRenderedSemaphores_[frameIndex];
        const auto imageAcquiredFence     = imageAcquiredFences_[frameIndex];
        const auto imageRenderedFence     = imageRenderedFences_[frameIndex];

        const auto fences = std::vector<VkFence> {imageAcquiredFence, imageRenderedFence};

        vkWaitForFences(device_, fences.size(), fences.data(), VK_TRUE, UINT64_MAX);
        vkResetFences(device_, fences.size(), fences.data());

        const auto imageIndex = getNextSwapchainImage(device_, swapchain_, imageAcquiredSemaphore, imageAcquiredFence);

        const auto commandBuffers   = std::vector<VkCommandBuffer> {commandBuffers_[frameIndex]};
        const auto waitDstStageMask = std::vector<VkPipelineStageFlags> {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        const auto waitSemaphores   = std::vector<VkSemaphore> {imageAcquiredSemaphore};
        const auto signalSemaphores = std::vector<VkSemaphore> {imageRenderedSemaphore};

        const auto submitInfo = getSubmitInfo({commandBuffers, waitDstStageMask, waitSemaphores, signalSemaphores});

        ND_ASSERT(vkQueueSubmit(deviceQueue, 1, &submitInfo, imageRenderedFence) == VK_SUCCESS);

        const auto swapchains   = std::vector<VkSwapchainKHR> {swapchain_};
        const auto imageIndices = std::vector<uint32_t> {static_cast<uint32_t>(frameIndex)};

        const auto presentInfo = getPresentInfo({swapchains, signalSemaphores, imageIndices});

        ND_ASSERT(vkQueuePresentKHR(swapchainQueue, &presentInfo) == VK_SUCCESS);

        frameIndex = (frameIndex + 1) % framesCount_;
    }

    VulkanContext
    getVulkanContext(const VulkanContextConfigurationExternal& configurationExternal,
                     const VulkanContextInitializers&          initializers,
                     const VulkanContextConfigurations&        configurations)
    {
        ND_SET_SCOPE();

        using std::string;
        using std::vector;

        const auto instanceConfiguration = configurations.getInstanceConfiguration(configurationExternal);
        const auto instance              = initializers.getInstance(instanceConfiguration, {}, {});

        const auto physicalDeviceConfiguration = configurations.getPhysicalDeviceConfiguration();
        const auto physicalDevice              = initializers.getPhysicalDevice(physicalDeviceConfiguration, instance);

        const auto [deviceQueueFamilies, device] =
            getDevice({physicalDeviceConfiguration.features, physicalDeviceConfiguration.extensions}, physicalDevice);

        const auto deviceQueues = getQueues(device, deviceQueueFamilies);

        const auto surface = initializers.getSurface(instance);

        const auto swapchainQueueFamilies = getSwapchainQueueFamilies(deviceQueueFamilies, physicalDevice, surface);
        const auto swapchainQueues        = getQueues(device, swapchainQueueFamilies);

        const auto swapchainConfiguration =
            SwapchainConfiguration {swapchainQueueFamilies,
                                    physicalDevice,
                                    surface,
                                    {configurationExternal.width, configurationExternal.height},
                                    1,
                                    1,
                                    true,
                                    VK_FORMAT_B8G8R8A8_SRGB,
                                    VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
                                    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                    VK_PRESENT_MODE_IMMEDIATE_KHR,
                                    VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                                    VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR};

        const auto swapchain = getSwapchain(swapchainConfiguration, device);

        const auto colorAttachments = vector<VkAttachmentReference> {{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}};

        const auto renderPassAttachments =
            vector<VkAttachmentDescription> {getRenderPassAttachment(swapchainConfiguration.imageFormat,
                                                                     VK_SAMPLE_COUNT_1_BIT,
                                                                     VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                                     VK_ATTACHMENT_STORE_OP_STORE,
                                                                     VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                                     VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                                     VK_IMAGE_LAYOUT_UNDEFINED,
                                                                     VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)};

        const auto renderPassSubpasses = vector<VkSubpassDescription> {getRenderPassSubpass(VK_PIPELINE_BIND_POINT_GRAPHICS,
                                                                                            0,
                                                                                            colorAttachments.size(),
                                                                                            0,
                                                                                            nullptr,
                                                                                            colorAttachments.data(),
                                                                                            nullptr,
                                                                                            nullptr,
                                                                                            nullptr)};

        const auto renderPassDependencies =
            vector<VkSubpassDependency> {getRenderPassDependency(VK_SUBPASS_EXTERNAL,
                                                                 0,
                                                                 VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                                                 VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                                                 0,
                                                                 VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                                                                 {})};

        const auto renderPass = getRenderPass({renderPassAttachments, renderPassSubpasses, renderPassDependencies}, device);

        const auto swapchainImages = getSwapchainImages(device, swapchain);
        const auto swapchainImageViews =
            getMapped<VkImage, VkImageView>(swapchainImages,
                                            [device = device, &swapchainConfiguration](const auto image, const auto index)
                                            {
                                                return getImageView({{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
                                                                     {VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                      VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                      VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                      VK_COMPONENT_SWIZZLE_IDENTITY},
                                                                     VK_IMAGE_VIEW_TYPE_2D,
                                                                     swapchainConfiguration.imageFormat,
                                                                     image},
                                                                    device);
                                            });

        const auto swapchainFramebuffers = getMapped<VkImageView, VkFramebuffer>(
            swapchainImageViews,
            [device = device, renderPass, &swapchainConfiguration](const auto imageView, const auto index)
            {
                return getFramebuffer({{imageView},
                                       renderPass,
                                       swapchainConfiguration.imageExtent.width,
                                       swapchainConfiguration.imageExtent.height,
                                       swapchainConfiguration.imageArrayLayers},
                                      device);
            });

        const auto shaderPaths =
            vector<string> {"src/graphics/vulkan/shaders/vert.spv", "src/graphics/vulkan/shaders/frag.spv"};

        const auto shaderStages = vector<VkShaderStageFlagBits> {VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT};

        const auto shaderModules = getMapped<string, VkShaderModule>(shaderPaths,
                                                                     [device = device](const auto& path, const auto index)
                                                                     {
                                                                         return getShaderModule({path}, device);
                                                                     });

        const auto descriptorPool = getDescriptorPool({{{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1}}, 1},
                                                      device,
                                                      VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);

        const auto descriptorSetLayout = getDescriptorSetLayout(DescriptorSetLayoutConfiguration {{}}, device);
        const auto descriptorSets      = getDescriptorSet({{descriptorSetLayout}, descriptorPool}, device);

        const auto pipelineLayout = getPipelineLayout({{descriptorSetLayout}, {}}, device);

        const auto vertexInputStateCreateInfo = getPipelineVertexInputStateCreateInfo(0, 0, nullptr, nullptr);
        const auto inputAssemblyStateCreateInfo =
            getPipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

        const auto tessellationStateCreateInfo = VkPipelineTessellationStateCreateInfo {};

        const auto scissors =
            VkRect2D {{0, 0}, {swapchainConfiguration.imageExtent.width, swapchainConfiguration.imageExtent.height}};
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

        const auto multisampleStateCreateInfo =
            getPipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f, nullptr, VK_FALSE, VK_FALSE);

        const auto depthStencilStateCreateInfo = VkPipelineDepthStencilStateCreateInfo {};

        const auto blendConstants       = vector<float> {0.0f, 0.0f, 0.0f, 0.0f};
        const auto colorBlendAttachment = VkPipelineColorBlendAttachmentState {
            VK_TRUE,
            VK_BLEND_FACTOR_SRC_ALPHA,
            VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            VK_BLEND_OP_ADD,
            VK_BLEND_FACTOR_ONE,
            VK_BLEND_FACTOR_ZERO,
            VK_BLEND_OP_ADD,
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};

        const auto colorBlendStateCreateInfo =
            getPipelineColorBlendStateCreateInfo(VK_FALSE, VK_LOGIC_OP_OR, 1, &colorBlendAttachment, blendConstants.data());

        const auto dynamicStateCreateInfo = getPipelineDynamicStateCreateInfo(0, nullptr);

        const auto pipelines = getGraphicsPipeline(
            {{getMapped<VkShaderModule, VkPipelineShaderStageCreateInfo>(
                  shaderModules,
                  [&shaderStages](const auto& shaderModule, const auto index)
                  {
                      return getPipelineShaderStageCreateInfo(shaderStages[index], shaderModule, "main", nullptr);
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
              pipelineLayout,
              renderPass,
              0}},
            device);

        const auto graphicsQueueFamily = std::find_if(deviceQueueFamilies.begin(),
                                                      deviceQueueFamilies.end(),
                                                      [](const auto& queueFamily)
                                                      {
                                                          return isSubmask(queueFamily.queueFlags, VK_QUEUE_GRAPHICS_BIT);
                                                      });

        ND_ASSERT(graphicsQueueFamily != deviceQueueFamilies.end());

        const auto commandPool    = getCommandPool({graphicsQueueFamily->index}, device);
        const auto commandBuffers = getCommandBuffer(
            {commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, static_cast<uint32_t>(swapchainFramebuffers.size())},
            device);

        for(size_t i = 0; i < commandBuffers.size(); ++i)
        {
            const auto framebuffer   = swapchainFramebuffers[i];
            const auto commandBuffer = commandBuffers[i];

            const auto commandBufferBeginInfo = getCommandBufferBeginInfo(nullptr);

            ND_ASSERT(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) == VK_SUCCESS);

            const auto clearValues = vector<VkClearValue> {{{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f}}};
            const auto renderPassBeginInfo =
                getRenderPassBeginInfo(renderPass, framebuffer, scissors, clearValues.size(), clearValues.data());

            vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[0]);
            vkCmdDraw(commandBuffer, 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffer);

            ND_ASSERT(vkEndCommandBuffer(commandBuffer) == VK_SUCCESS);
        }

        const auto framesCount = size_t {2};

        const auto imageAcquiredSemaphores = getSemaphore(device, framesCount);
        const auto imageRenderedSemaphores = getSemaphore(device, framesCount);
        const auto imageAcquiredFences     = getFence(device, framesCount, VK_FENCE_CREATE_SIGNALED_BIT);
        const auto imageRenderedFences     = getFence(device, framesCount, VK_FENCE_CREATE_SIGNALED_BIT);

        return VulkanContext({deviceQueues,
                              swapchainQueues,
                              deviceQueueFamilies,
                              swapchainQueueFamilies,
                              swapchainImages,
                              swapchainImageViews,
                              swapchainFramebuffers,
                              shaderModules,
                              descriptorSets,
                              pipelines,
                              commandBuffers,
                              imageAcquiredSemaphores,
                              imageRenderedSemaphores,
                              imageAcquiredFences,
                              imageRenderedFences,
                              framesCount,
                              instance,
                              device,
                              surface,
                              swapchain,
                              renderPass,
                              descriptorPool,
                              descriptorSetLayout,
                              pipelineLayout,
                              commandPool});
    }
} // namespace nd::src::graphics::vulkan
