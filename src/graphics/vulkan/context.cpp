#include "context.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VulkanObjects::VulkanObjects(Instance                           instance,
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
                                 std::vector<size_t>&&              imageMemoryIndices)
        : instance {instance}
        , surface {surface}
        , renderPass {renderPass}
        , descriptorPool {descriptorPool}
        , physicalDevice {std::move(physicalDevice)}
        , device {std::move(device)}
        , swapchain {std::move(swapchain)}
        , deviceMemories {std::move(deviceMemories)}
        , swapchainImages {std::move(swapchainImages)}
        , swapchainImageViews {std::move(swapchainImageViews)}
        , swapchainFramebuffers {std::move(swapchainFramebuffers)}
        , shaderModules {std::move(shaderModules)}
        , descriptorSetLayouts {std::move(descriptorSetLayouts)}
        , descriptorSets {std::move(descriptorSets)}
        , pipelineLayouts {std::move(pipelineLayouts)}
        , graphicsPipelines {std::move(graphicsPipelines)}
        , commandPools {std::move(commandPools)}
        , commandBuffers {std::move(commandBuffers)}
        , buffers {std::move(buffers)}
        , bufferMemoryIndices {std::move(bufferMemoryIndices)}
        , imageMemoryIndices {std::move(imageMemoryIndices)}
    {
        ND_SET_SCOPE();

        instance       = VK_NULL_HANDLE;
        physicalDevice = {};
        device         = {};
        surface        = VK_NULL_HANDLE;
        swapchain      = {};
        renderPass     = VK_NULL_HANDLE;
        descriptorPool = VK_NULL_HANDLE;
    }

    VulkanObjects::VulkanObjects(VulkanObjects&& vulkanObjects) noexcept
        : instance {vulkanObjects.instance}
        , surface {vulkanObjects.surface}
        , renderPass {vulkanObjects.renderPass}
        , descriptorPool {vulkanObjects.descriptorPool}
        , physicalDevice {std::move(vulkanObjects.physicalDevice)}
        , device {std::move(vulkanObjects.device)}
        , swapchain {std::move(vulkanObjects.swapchain)}
        , deviceMemories {std::move(vulkanObjects.deviceMemories)}
        , swapchainImages {std::move(vulkanObjects.swapchainImages)}
        , swapchainImageViews {std::move(vulkanObjects.swapchainImageViews)}
        , swapchainFramebuffers {std::move(vulkanObjects.swapchainFramebuffers)}
        , shaderModules {std::move(vulkanObjects.shaderModules)}
        , descriptorSetLayouts {std::move(vulkanObjects.descriptorSetLayouts)}
        , descriptorSets {std::move(vulkanObjects.descriptorSets)}
        , pipelineLayouts {std::move(vulkanObjects.pipelineLayouts)}
        , graphicsPipelines {std::move(vulkanObjects.graphicsPipelines)}
        , commandPools {std::move(vulkanObjects.commandPools)}
        , commandBuffers {std::move(vulkanObjects.commandBuffers)}
        , buffers {std::move(vulkanObjects.buffers)}
        , bufferMemoryIndices {std::move(vulkanObjects.bufferMemoryIndices)}
        , imageMemoryIndices {std::move(vulkanObjects.imageMemoryIndices)}
        , semaphores {std::move(vulkanObjects.semaphores)}
        , fences {std::move(vulkanObjects.fences)}
    {
        ND_SET_SCOPE();

        vulkanObjects.instance       = VK_NULL_HANDLE;
        vulkanObjects.physicalDevice = {};
        vulkanObjects.device         = {};
        vulkanObjects.surface        = VK_NULL_HANDLE;
        vulkanObjects.swapchain      = {};
        vulkanObjects.renderPass     = VK_NULL_HANDLE;
        vulkanObjects.descriptorPool = VK_NULL_HANDLE;
    }

    VulkanObjects&
    VulkanObjects::operator=(VulkanObjects&& vulkanObjects) noexcept
    {
        ND_SET_SCOPE();

        if(this == &vulkanObjects)
        {
            return *this;
        }

        instance       = vulkanObjects.instance;
        surface        = vulkanObjects.surface;
        renderPass     = vulkanObjects.renderPass;
        descriptorPool = vulkanObjects.descriptorPool;

        physicalDevice = std::move(vulkanObjects.physicalDevice);
        device         = std::move(vulkanObjects.device);
        swapchain      = std::move(vulkanObjects.swapchain);

        deviceMemories        = std::move(vulkanObjects.deviceMemories);
        swapchainImages       = std::move(vulkanObjects.swapchainImages);
        swapchainImageViews   = std::move(vulkanObjects.swapchainImageViews);
        swapchainFramebuffers = std::move(vulkanObjects.swapchainFramebuffers);
        shaderModules         = std::move(vulkanObjects.shaderModules);

        descriptorSetLayouts = std::move(vulkanObjects.descriptorSetLayouts);
        descriptorSets       = std::move(vulkanObjects.descriptorSets);
        pipelineLayouts      = std::move(vulkanObjects.pipelineLayouts);
        graphicsPipelines    = std::move(vulkanObjects.graphicsPipelines);
        commandPools         = std::move(vulkanObjects.commandPools);
        commandBuffers       = std::move(vulkanObjects.commandBuffers);
        buffers              = std::move(vulkanObjects.buffers);

        bufferMemoryIndices = std::move(vulkanObjects.bufferMemoryIndices);
        imageMemoryIndices  = std::move(vulkanObjects.imageMemoryIndices);

        semaphores = std::move(vulkanObjects.semaphores);
        fences     = std::move(vulkanObjects.fences);

        vulkanObjects.instance       = VK_NULL_HANDLE;
        vulkanObjects.surface        = VK_NULL_HANDLE;
        vulkanObjects.renderPass     = VK_NULL_HANDLE;
        vulkanObjects.descriptorPool = VK_NULL_HANDLE;

        vulkanObjects.physicalDevice = {};
        vulkanObjects.device         = {};
        vulkanObjects.swapchain      = {};

        return *this;
    }

    VulkanObjects::~VulkanObjects()
    {
        ND_SET_SCOPE();

        if(instance == VK_NULL_HANDLE)
        {
            return;
        }

        vkDeviceWaitIdle(device.handle);

        if(fences.size())
        {
            vkWaitForFences(device.handle, fences.size(), fences.data(), true, UINT64_MAX);
        }

        for(const auto semaphore: semaphores)
        {
            vkDestroySemaphore(device.handle, semaphore, nullptr);
        }

        for(const auto fence: fences)
        {
            vkDestroyFence(device.handle, fence, nullptr);
        }

        for(const auto buffer: buffers)
        {
            vkDestroyBuffer(device.handle, buffer.handle, nullptr);
        }

        for(auto index = 0; index < commandPools.size(); ++index)
        {
            vkFreeCommandBuffers(device.handle, commandPools[index].handle, commandBuffers[index].size(), commandBuffers[index].data());

            vkDestroyCommandPool(device.handle, commandPools[index].handle, nullptr);
        }

        for(const auto graphicsPipeline: graphicsPipelines)
        {
            vkDestroyPipeline(device.handle, graphicsPipeline, nullptr);
        }

        for(const auto pipelineLayout: pipelineLayouts)
        {
            vkDestroyPipelineLayout(device.handle, pipelineLayout, nullptr);
        }

        vkFreeDescriptorSets(device.handle, descriptorPool, descriptorSets.size(), descriptorSets.data());

        for(const auto descriptorSetLayout: descriptorSetLayouts)
        {
            vkDestroyDescriptorSetLayout(device.handle, descriptorSetLayout, nullptr);
        }

        vkDestroyDescriptorPool(device.handle, descriptorPool, nullptr);

        for(const auto shaderModule: shaderModules)
        {
            vkDestroyShaderModule(device.handle, shaderModule.handle, nullptr);
        }

        for(const auto swapchainFramebuffer: swapchainFramebuffers)
        {
            vkDestroyFramebuffer(device.handle, swapchainFramebuffer, nullptr);
        }

        for(const auto swapchainImageView: swapchainImageViews)
        {
            vkDestroyImageView(device.handle, swapchainImageView, nullptr);
        }

        vkDestroyRenderPass(device.handle, renderPass, nullptr);
        vkDestroySwapchainKHR(device.handle, swapchain.handle, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);

        for(const auto deviceMemory: deviceMemories)
        {
            vkFreeMemory(device.handle, deviceMemory.handle, nullptr);
        }

        vkDestroyDevice(device.handle, nullptr);
        vkDestroyInstance(instance, nullptr);
    }

    VulkanContext::VulkanContext(VulkanObjects&& objects)
        : objects_ {std::move(objects)}
    {
        ND_SET_SCOPE();
    }

    const QueueFamily&
    VulkanContext::getGraphicsQueueFamily() const
    {
        ND_SET_SCOPE();

        static const auto queueFamily = getQueueFamily(objects_.device.queueFamilies, VK_QUEUE_GRAPHICS_BIT);

        ND_ASSERT(queueFamily != objects_.device.queueFamilies.end());

        return *queueFamily;
    }

    const QueueFamily&
    VulkanContext::getComputeQueueFamily() const
    {
        ND_SET_SCOPE();

        static const auto queueFamily = getQueueFamily(objects_.device.queueFamilies, VK_QUEUE_COMPUTE_BIT);

        ND_ASSERT(queueFamily != objects_.device.queueFamilies.end());

        return *queueFamily;
    }

    const QueueFamily&
    VulkanContext::getTransferQueueFamily() const
    {
        ND_SET_SCOPE();

        static const auto queueFamily = getQueueFamily(objects_.device.queueFamilies,
                                                       VK_QUEUE_TRANSFER_BIT,
                                                       VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);

        ND_ASSERT(queueFamily != objects_.device.queueFamilies.end());

        return *queueFamily;
    }

    const QueueFamily&
    VulkanContext::getSwapchainQueueFamily() const
    {
        ND_SET_SCOPE();

        ND_ASSERT(objects_.swapchain.queueFamilies.size());

        static const auto queueFamily = objects_.swapchain.queueFamilies.front();

        return queueFamily;
    }

    const CommandBuffers&
    VulkanContext::getGraphicsCommandBuffers() const
    {
        ND_SET_SCOPE();

        return objects_.commandBuffers[0];
    }

    const CommandBuffers&
    VulkanContext::getTransferCommandBuffers() const
    {
        ND_SET_SCOPE();

        return objects_.commandBuffers[1];
    }

    constexpr int
    VulkanContext::getVertexBufferIndex() const
    {
        return 0;
    }

    constexpr int
    VulkanContext::getIndexBufferIndex() const
    {
        return 1;
    }

    constexpr int
    VulkanContext::getStagingBufferIndex() const
    {
        return 2;
    }

    const Buffer&
    VulkanContext::getVertexBuffer() const
    {
        ND_SET_SCOPE();

        return objects_.buffers[getVertexBufferIndex()];
    }

    const Buffer&
    VulkanContext::getIndexBuffer() const
    {
        ND_SET_SCOPE();

        return objects_.buffers[getIndexBufferIndex()];
    }

    const Buffer&
    VulkanContext::getStagingBuffer() const
    {
        ND_SET_SCOPE();

        return objects_.buffers[getStagingBufferIndex()];
    }

    VkSemaphore
    VulkanContext::getSemaphore(const VkSemaphoreCreateFlags flags, const void* next) noexcept
    {
        ND_SET_SCOPE();

        const auto semaphore = nd::src::graphics::vulkan::getSemaphore(objects_.device.handle, flags, next);

        objects_.semaphores.push_back(semaphore);

        return semaphore;
    }

    std::vector<VkSemaphore>
    VulkanContext::getSemaphores(const size_t count, const VkSemaphoreCreateFlags flags, const void* next) noexcept
    {
        ND_SET_SCOPE();

        auto semaphores = nd::src::graphics::vulkan::getSemaphore(objects_.device.handle, count, flags, next);

        objects_.semaphores.insert(objects_.semaphores.end(), semaphores.begin(), semaphores.end());

        return semaphores;
    }

    VkFence
    VulkanContext::getFence(const VkFenceCreateFlags flags, const void* next) noexcept
    {
        ND_SET_SCOPE();

        const auto fence = nd::src::graphics::vulkan::getFence(objects_.device.handle, flags, next);

        objects_.fences.push_back(fence);

        return fence;
    }

    std::vector<VkFence>
    VulkanContext::getFences(const size_t count, const VkFenceCreateFlags flags, const void* next) noexcept
    {
        ND_SET_SCOPE();

        auto fences = nd::src::graphics::vulkan::getFence(objects_.device.handle, count, flags, next);

        objects_.fences.insert(objects_.fences.end(), fences.begin(), fences.end());

        return fences;
    }

    void
    VulkanContext::drawNextFrame()
    {
        ND_SET_SCOPE();

        static auto frameIndex = size_t {0};
        static auto loaded     = false;

        if(!loaded)
        {
            const auto indices  = std::vector<uint16_t> {0, 1, 2, 2, 3, 0};
            const auto vertices = std::vector<Vertex> {{{0.0, -0.5, 0.0}, {1.0, 0.0, 0.0}},
                                                       {{0.5, 0.5, 0.0}, {0.0, 1.0, 0.0}},
                                                       {{-0.5, 0.5, 0.0}, {0.0, 0.0, 1.0}},
                                                       {{-0.5, -0.5, 0.0}, {0.0, 1.0, 0.0}}};

            const auto stagingBufferMemoryIndex = objects_.bufferMemoryIndices[getStagingBufferIndex()];
            const auto stagingBufferMemory      = objects_.deviceMemories[stagingBufferMemoryIndex];

            const auto indexBufferSize    = sizeof(uint16_t) * indices.size();
            const auto vertexBufferSize   = sizeof(Vertex) * vertices.size();
            const auto indexBufferOffset  = 0;
            const auto vertexBufferOffset = indexBufferOffset + indexBufferSize;

            setMemory(objects_.device.handle, stagingBufferMemory, indexBufferOffset, indexBufferSize, indices.data());
            setMemory(objects_.device.handle, stagingBufferMemory, vertexBufferOffset, vertexBufferSize, vertices.data());

            const auto transferQueue                  = getTransferQueueFamily().queues.front();
            const auto transferCommandBuffer          = getTransferCommandBuffers().front();
            const auto transferCommandBufferBeginInfo = getCommandBufferBeginInfo(nullptr);

            vkBeginCommandBuffer(transferCommandBuffer, &transferCommandBufferBeginInfo);

            const auto& indexBuffer   = getIndexBuffer();
            const auto& vertexBuffer  = getVertexBuffer();
            const auto& stagingBuffer = getStagingBuffer();

            const auto indexRegions  = std::array {VkBufferCopy {indexBufferOffset, 0, indexBufferSize}};
            const auto vertexRegions = std::array {VkBufferCopy {vertexBufferOffset, 0, vertexBufferSize}};

            vkCmdCopyBuffer(transferCommandBuffer, stagingBuffer.handle, indexBuffer.handle, indexRegions.size(), indexRegions.data());
            vkCmdCopyBuffer(transferCommandBuffer, stagingBuffer.handle, vertexBuffer.handle, vertexRegions.size(), vertexRegions.data());

            vkEndCommandBuffer(transferCommandBuffer);

            auto transferSubmitInfo = VkSubmitInfo {};

            transferSubmitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            transferSubmitInfo.commandBufferCount = 1;
            transferSubmitInfo.pCommandBuffers    = &transferCommandBuffer;

            vkQueueSubmit(transferQueue, 1, &transferSubmitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(transferQueue);

            loaded = true;
        }

        static const auto graphicsQueue  = getGraphicsQueueFamily().queues.front();
        static const auto swapchainQueue = getSwapchainQueueFamily().queues.front();

        static const auto imageAcquiredSemaphores = getSemaphores(objects_.swapchainImages.size());
        static const auto imageRenderedSemaphores = getSemaphores(objects_.swapchainImages.size());
        static const auto imageAcquiredFences     = getFences(objects_.swapchainImages.size(), VK_FENCE_CREATE_SIGNALED_BIT);
        static const auto imageRenderedFences     = getFences(objects_.swapchainImages.size(), VK_FENCE_CREATE_SIGNALED_BIT);

        const auto imageAcquiredSemaphore = imageAcquiredSemaphores[frameIndex];
        const auto imageRenderedSemaphore = imageRenderedSemaphores[frameIndex];
        const auto imageAcquiredFence     = imageAcquiredFences[frameIndex];
        const auto imageRenderedFence     = imageRenderedFences[frameIndex];

        const auto fences = std::array {imageAcquiredFence, imageRenderedFence};

        vkWaitForFences(objects_.device.handle, fences.size(), fences.data(), VK_TRUE, UINT64_MAX);
        vkResetFences(objects_.device.handle, fences.size(), fences.data());

        const auto imageIndex = getNextSwapchainImage(objects_.device.handle, objects_.swapchain.handle, imageAcquiredSemaphore, imageAcquiredFence);

        const auto commandBuffers   = std::array {objects_.commandBuffers[0][imageIndex]};
        const auto waitDstStageMask = std::array {static_cast<VkPipelineStageFlags>(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)};
        const auto waitSemaphores   = std::array {imageAcquiredSemaphore};
        const auto signalSemaphores = std::array {imageRenderedSemaphore};

        const auto submitInfos = std::array {getSubmitInfo({commandBuffers, waitDstStageMask, waitSemaphores, signalSemaphores})};

        ND_ASSERT_EXEC(vkQueueSubmit(graphicsQueue, submitInfos.size(), submitInfos.data(), imageRenderedFence) == VK_SUCCESS);

        const auto swapchains   = std::array {objects_.swapchain.handle};
        const auto imageIndices = std::array {static_cast<uint32_t>(imageIndex)};

        const auto presentInfo = getPresentInfo({swapchains, signalSemaphores, imageIndices});

        ND_ASSERT_EXEC(vkQueuePresentKHR(swapchainQueue, &presentInfo) == VK_SUCCESS);

        frameIndex = (frameIndex + 1) % objects_.swapchainImages.size();
    }

    VulkanObjects
    getVulkanObjects(const VulkanConfigurationExternal& configurationExternal,
                     const VulkanInitializers&          initializers,
                     const VulkanConfigurations&        configurations)
    {
        ND_SET_SCOPE();

        const auto width  = configurationExternal.width;
        const auto height = configurationExternal.height;

        const auto instanceConfig = configurations.getInstance(configurationExternal);
        auto       instance       = initializers.getInstance(instanceConfig);

        const auto physicalDeviceConfig = configurations.getPhysicalDevice();
        auto       physicalDevice       = initializers.getPhysicalDevice(physicalDeviceConfig, instance);

        const auto deviceConfig = configurations.getDevice(physicalDeviceConfig);
        auto       device       = initializers.getDevice(deviceConfig, physicalDevice.handle);

        const auto deviceMemoryConfigs = configurations.getDeviceMemories(physicalDevice.memoryProperties.get());
        auto       deviceMemories      = initializers.getDeviceMemories(deviceMemoryConfigs, device.handle);

        auto surface = initializers.getSurface(instance);

        const auto swapchainConfig = configurations.getSwapchain(physicalDevice.handle, surface, width, height);
        auto       swapchain       = initializers.getSwapchain(swapchainConfig, device.handle);

        const auto renderPassConfig = configurations.getRenderPass(swapchainConfig);
        auto       renderPass       = initializers.getRenderPass(renderPassConfig, device.handle);

        auto swapchainImages = initializers.getSwapchainImages(swapchain.handle, device.handle);

        const auto swapchainImageViewConfigs = configurations.getSwapchainImageViews(swapchainConfig, swapchainImages);
        auto       swapchainImageViews       = initializers.getSwapchainImageViews(swapchainImageViewConfigs, device.handle);

        const auto swapchainFramebufferConfigs = configurations.getSwapchainFramebuffers(swapchainConfig, swapchainImageViews, renderPass);
        auto       swapchainFramebuffers       = initializers.getSwapchainFramebuffers(swapchainFramebufferConfigs, device.handle);

        const auto shaderModuleConfigs = configurations.getShaderModules();
        auto       shaderModules       = initializers.getShaderModules(shaderModuleConfigs, device.handle);

        const auto descriptorPoolConfig = configurations.getDescriptorPool();
        auto       descriptorPool       = initializers.getDescriptorPool(descriptorPoolConfig, device.handle);

        const auto descriptorSetLayoutConfigs = configurations.getDescriptorSetLayouts();
        auto       descriptorSetLayouts       = initializers.getDescriptorSetLayouts(descriptorSetLayoutConfigs, device.handle);

        const auto descriptorSetConfig = configurations.getDescriptorSet(descriptorSetLayouts, descriptorPool);
        auto       descriptorSets      = initializers.getDescriptorSets(descriptorSetConfig, device.handle);

        const auto pipelineLayoutConfigs = configurations.getPipelineLayouts(descriptorSetLayouts);
        auto       pipelineLayouts       = initializers.getPipelineLayouts(pipelineLayoutConfigs, device.handle);

        const auto graphicsPipelineConfigs = configurations.getGraphicsPipelines(shaderModules, pipelineLayouts, renderPass, width, height);
        auto       graphicsPipelines       = initializers.getGraphicsPipelines(graphicsPipelineConfigs, device.handle);

        const auto commandPoolConfigs = configurations.getCommandPools(device.queueFamilies);
        auto       commandPools       = initializers.getCommandPools(commandPoolConfigs, device.handle);

        const auto commandBufferConfigs = configurations.getCommandBuffers(commandPools);
        auto       commandBuffers       = initializers.getCommandBuffers(commandBufferConfigs, device.handle);

        const auto bufferConfigs = configurations.getBuffers(device.queueFamilies);
        auto       buffers       = initializers.getBuffers(bufferConfigs, device.handle);

        auto bufferMemoryIndices = initializers.bindBufferMemory(device.handle, physicalDevice.memoryProperties.get(), deviceMemories, buffers);
        auto imageMemoryIndices  = initializers.bindImageMemory(device.handle,
                                                               physicalDevice.memoryProperties.get(),
                                                               deviceMemories,
                                                               std::vector<Image> {});

        for(auto i = 0; i < swapchainFramebuffers.size(); ++i)
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

            const auto vertexBuffers        = std::array {buffers[0].handle};
            const auto vertexBuffersOffsets = std::array {VkDeviceSize {0}};

            vkCmdBindVertexBuffers(commandBuffer, 0, vertexBuffers.size(), vertexBuffers.data(), vertexBuffersOffsets.data());
            vkCmdBindIndexBuffer(commandBuffer, buffers[1].handle, VkDeviceSize {0}, VK_INDEX_TYPE_UINT16);

            vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);

            vkCmdEndRenderPass(commandBuffer);

            ND_ASSERT_EXEC(vkEndCommandBuffer(commandBuffer) == VK_SUCCESS);
        }

        return VulkanObjects {instance,
                              surface,
                              renderPass,
                              descriptorPool,
                              std::move(physicalDevice),
                              std::move(device),
                              std::move(swapchain),
                              std::move(deviceMemories),
                              std::move(swapchainImages),
                              std::move(swapchainImageViews),
                              std::move(swapchainFramebuffers),
                              std::move(shaderModules),
                              std::move(descriptorSetLayouts),
                              std::move(descriptorSets),
                              std::move(pipelineLayouts),
                              std::move(graphicsPipelines),
                              std::move(commandPools),
                              std::move(commandBuffers),
                              std::move(buffers),
                              std::move(bufferMemoryIndices),
                              std::move(imageMemoryIndices)};
    }

    VulkanContext
    getVulkanContext(const VulkanConfigurationExternal& configurationExternal,
                     const VulkanInitializers&          initializers,
                     const VulkanConfigurations&        configurations)
    {
        ND_SET_SCOPE();

        return VulkanContext {getVulkanObjects(configurationExternal, initializers, configurations)};
    }
} // namespace nd::src::graphics::vulkan
