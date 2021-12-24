#include "context.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VulkanContext::VulkanContext(const Configuration& configuration)
        : instance_(std::move(configuration.instance))
        , device_(std::move(configuration.device))
        , surface_(std::move(configuration.surface))
        , swapchain_(std::move(configuration.swapchain))
        , renderPass_(std::move(configuration.renderPass))
        , swapchainImages_(std::move(configuration.swapchainImages))
        , swapchainImageViews_(std::move(configuration.swapchainImageViews))
        , swapchainFramebuffers_(std::move(configuration.swapchainFramebuffers))
        , shaderModules_(std::move(configuration.shaderModules))
        , descriptorPool_(std::move(configuration.descriptorPool))
        , descriptorSetLayout_(std::move(configuration.descriptorSetLayout))
        , descriptorSets_(std::move(configuration.descriptorSets))
        , pipelineLayout_(std::move(configuration.pipelineLayout))
        , pipelines_(std::move(configuration.pipelines))
        , commandPool_(std::move(configuration.commandPool))
        , commandBuffers_(std::move(configuration.commandBuffers))
        , framesCount_(std::move(configuration.framesCount))
        , imageAcquiredSemaphores_(std::move(configuration.imageAcquiredSemaphores))
        , imageRenderedSemaphores_(std::move(configuration.imageRenderedSemaphores))
        , imageAcquiredFences_(std::move(configuration.imageAcquiredFences))
        , imageRenderedFences_(std::move(configuration.imageRenderedFences))
        , graphicsQueue_(configuration.graphicsQueue)
        , presentQueue_(configuration.presentQueue)
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

        const auto imageAcquiredSemaphore = imageAcquiredSemaphores_[frameIndex];
        const auto imageRenderedSemaphore = imageRenderedSemaphores_[frameIndex];
        const auto imageAcquiredFence     = imageAcquiredFences_[frameIndex];
        const auto imageRenderedFence     = imageRenderedFences_[frameIndex];

        vkWaitForFences(device_, 1u, &imageRenderedFence, VK_TRUE, UINT64_MAX);
        vkResetFences(device_, 1u, &imageRenderedFence);

        const auto imageIndex = getNextSwapchainImage(device_, swapchain_, imageAcquiredSemaphore, VK_NULL_HANDLE);

        const auto commandBuffers   = std::vector<VkCommandBuffer> {commandBuffers_[frameIndex]};
        const auto waitDstStageMask = std::vector<VkPipelineStageFlags> {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        const auto waitSemaphores   = std::vector<VkSemaphore> {imageAcquiredSemaphore};
        const auto signalSemaphores = std::vector<VkSemaphore> {imageRenderedSemaphore};

        const auto submitInfo = getSubmitInfo({commandBuffers, waitDstStageMask, waitSemaphores, signalSemaphores});

        ND_ASSERT(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, imageRenderedFence) == VK_SUCCESS);

        const auto swapchains   = std::vector<VkSwapchainKHR> {swapchain_};
        const auto imageIndices = std::vector<uint32_t> {static_cast<uint32_t>(frameIndex)};

        const auto presentInfo = getPresentInfo({swapchains, signalSemaphores, imageIndices});

        ND_ASSERT(vkQueuePresentKHR(presentQueue_, &presentInfo) == VK_SUCCESS);

        frameIndex = (frameIndex + 1) % framesCount_;
    }

    VulkanContext
    getVulkanContext(const VulkanContextConfiguration& configuration)
    {
        ND_SET_SCOPE();

        using std::string;
        using std::vector;

        const auto layers = getMerged(configuration.layers,
                                      {
#ifndef NDEBUG
                                          "VK_LAYER_KHRONOS_validation"
#endif
                                      });

        const auto extensions = getMerged(configuration.extensions, {});

        const auto instance = getInstance({configuration.applicationName,
                                           configuration.engineName,
                                           layers,
                                           extensions,
                                           VK_MAKE_VERSION(0, 1, 0),
                                           VK_MAKE_VERSION(0, 1, 0),
                                           VK_API_VERSION_1_2});

        const auto physicalDevicePriority = [](const VkPhysicalDevice            physicalDevice,
                                               const VkPhysicalDeviceProperties& properties,
                                               const VkPhysicalDeviceFeatures&   features)
        {
            return 1;
        };

        const auto deviceFeatures   = VkPhysicalDeviceFeatures {};
        const auto deviceExtensions = vector<string> {"VK_KHR_swapchain"};
        const auto deviceQueueFlags = VK_QUEUE_GRAPHICS_BIT;

        const auto physicalDevice =
            getPhysicalDevice({deviceFeatures, physicalDevicePriority, deviceExtensions, deviceQueueFlags}, instance);

        const auto deviceQueueFamilies = getPhysicalDeviceQueueFamilies(physicalDevice, deviceQueueFlags);
        const auto device              = getDevice({deviceFeatures, deviceQueueFamilies, deviceExtensions}, physicalDevice);

        const auto surface = configuration.getSurface(instance);

        const auto swapchainQueueFamilies = getSwapchainQueueFamilies(deviceQueueFamilies, physicalDevice, surface);
        const auto swapchainConfiguration = SwapchainConfiguration {swapchainQueueFamilies,
                                                                    physicalDevice,
                                                                    surface,
                                                                    {configuration.width, configuration.height},
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

        auto swapchainImages = getSwapchainImages(device, swapchain);
        auto swapchainImageViews =
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

        auto swapchainFramebuffers = getMapped<VkImageView, VkFramebuffer>(
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

        auto shaderModules = getMapped<string, VkShaderModule>(shaderPaths,
                                                               [device = device](const auto& path, const auto index)
                                                               {
                                                                   return getShaderModule({path}, device);
                                                               });

        const auto descriptorPool = getDescriptorPool({{{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1}}, 1},
                                                      device,
                                                      VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);

        const auto descriptorSetLayout = getDescriptorSetLayout(DescriptorSetLayoutConfiguration {{}}, device);

        auto descriptorSets = getDescriptorSet({{descriptorSetLayout}, descriptorPool}, device);

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

        auto pipelines = getGraphicsPipeline(
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

        // TODO: Extract receiving of queue for command pool
        const auto commandPool = getCommandPool({deviceQueueFamilies, VK_QUEUE_GRAPHICS_BIT}, device);

        auto commandBuffers = getCommandBuffer(
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



        // TODO: Use queue families from 'deviceQueueFamilies' and 'swapchainQueueFamilies'
        // TODO: Remove getQueueFamily and getPresentQueueFamily
        const auto graphicsQueueFamily = getQueueFamily(deviceQueueFamilies, VK_QUEUE_GRAPHICS_BIT);
        const auto presentQueueFamily  = getPresentQueueFamily(deviceQueueFamilies, physicalDevice, surface);

        ND_ASSERT(graphicsQueueFamily.has_value() && presentQueueFamily.has_value());

        const auto graphicsQueue = getQueue(device, graphicsQueueFamily.value().index, 0);
        const auto presentQueue  = getQueue(device, presentQueueFamily.value().index, 0);



        const auto framesCount = size_t {2};

        auto imageAcquiredSemaphores = vector<VkSemaphore>(framesCount);
        auto imageRenderedSemaphores = vector<VkSemaphore>(framesCount);
        auto imageAcquiredFences     = vector<VkFence>(framesCount);
        auto imageRenderedFences     = vector<VkFence>(framesCount);

        for(size_t index = 0; index < framesCount; ++index)
        {
            imageAcquiredSemaphores[index] = getSemaphore(device);
            imageRenderedSemaphores[index] = getSemaphore(device);
            imageAcquiredFences[index]     = getFence(device);
            imageRenderedFences[index]     = getFence(device, VK_FENCE_CREATE_SIGNALED_BIT);
        }

        return VulkanContext({move(swapchainImages),
                              move(swapchainImageViews),
                              move(swapchainFramebuffers),
                              move(shaderModules),
                              move(descriptorSets),
                              move(pipelines),
                              move(commandBuffers),
                              move(imageAcquiredSemaphores),
                              move(imageRenderedSemaphores),
                              move(imageAcquiredFences),
                              move(imageRenderedFences),
                              framesCount,
                              instance,
                              device,
                              surface,
                              swapchain,
                              renderPass,
                              descriptorPool,
                              descriptorSetLayout,
                              pipelineLayout,
                              commandPool,
                              graphicsQueue,
                              presentQueue});
    }
} // namespace nd::src::graphics::vulkan
