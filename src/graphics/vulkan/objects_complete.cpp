#include "objects_complete.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VulkanObjects
    createVulkanObjects(opt<const Dependency>::ref dependency, opt<const VulkanObjectsCfg>::ref cfg, opt<const VulkanObjectsInit>::ref init) noexcept
    {
        ND_SET_SCOPE();

        const auto instanceCfg = cfg.instance(dependency);
        const auto instance    = init.instance(instanceCfg);

        const auto physicalDeviceCfg = cfg.physicalDevice();
        const auto physicalDevice    = init.physicalDevice(physicalDeviceCfg, instance);

        const auto deviceCfg = cfg.device(physicalDeviceCfg);
        const auto device    = init.device(deviceCfg, physicalDevice);

        const auto bufferCfg = cfg.buffer(device);
        const auto buffer    = init.buffer(bufferCfg, device.handle, physicalDevice);

        const auto surface = init.surface(instance);

        const auto swapchainCfg = cfg.swapchain(dependency, physicalDevice, device, surface);
        const auto swapchain    = init.swapchain(swapchainCfg, device.handle);

        const auto renderPassCfg = cfg.renderPass(swapchainCfg);
        const auto renderPass    = init.renderPass(renderPassCfg, device.handle);

        auto swapchainImages = init.swapchainImages(device.handle, swapchain.handle);

        const auto swapchainImageViewCfg = cfg.swapchainImageView(swapchainCfg);
        auto       swapchainImageViews   = init.swapchainImageViews(swapchainImageViewCfg, device.handle, swapchainImages);

        const auto swapchainFramebufferCfg = cfg.swapchainFramebuffer(swapchainCfg, renderPass);
        auto       swapchainFramebuffers   = init.swapchainFramebuffers(swapchainFramebufferCfg, device.handle, swapchainImageViews);

        const auto descriptorPoolCfg = cfg.descriptorPool();
        const auto descriptorPool    = init.descriptorPool(descriptorPoolCfg, device.handle);

        const auto descriptorSetLayoutCfg = cfg.descriptorSetLayout();
        const auto descriptorSetLayout    = init.descriptorSetLayout(descriptorSetLayoutCfg, device.handle);

        const auto shaderModulesCfg = cfg.shaderModules();
        auto       shaderModules    = init.shaderModules(shaderModulesCfg, device.handle);

        const auto pipelineCacheCfg = cfg.pipelineCache();
        const auto pipelineCache    = init.pipelineCache(pipelineCacheCfg, device.handle);

        const auto pipelineLayoutCfg = cfg.pipelineLayout(descriptorSetLayout);
        const auto pipelineLayout    = init.pipelineLayout(pipelineLayoutCfg, device.handle);

        const auto pipelineCfg = cfg.pipeline(swapchainCfg, renderPass, pipelineLayout, shaderModules);
        const auto pipeline    = init.pipeline(pipelineCfg, device.handle, pipelineCache);

        const auto commandPoolCfg = cfg.commandPool(device, swapchainImages.size());
        const auto commandPool    = init.commandPool(commandPoolCfg, device.handle);

        return {.buffer                = buffer,
                .device                = device,
                .swapchainImages       = std::move(swapchainImages),
                .swapchainImageViews   = std::move(swapchainImageViews),
                .swapchainFramebuffers = std::move(swapchainFramebuffers),
                .shaderModules         = std::move(shaderModules),
                .commandPool           = commandPool,
                .descriptorSetLayout   = descriptorSetLayout,
                .pipelineLayout        = pipelineLayout,
                .pipeline              = pipeline,
                .swapchain             = swapchain,
                .instance              = instance,
                .physicalDevice        = physicalDevice,
                .surface               = surface,
                .renderPass            = renderPass,
                .descriptorPool        = descriptorPool,
                .pipelineCache         = pipelineCache};
    }

    void
    destroyVulkanObjects(opt<const VulkanObjects>::ref objects) noexcept
    {
        ND_SET_SCOPE();

        for(opt<const Semaphore>::ref semaphore: objects.semaphores)
        {
            vkDestroySemaphore(objects.device.handle, semaphore, ND_VULKAN_ALLOCATION_CALLBACKS);
        }

        for(opt<const Fence>::ref fence: objects.fences)
        {
            vkDestroyFence(objects.device.handle, fence, ND_VULKAN_ALLOCATION_CALLBACKS);
        }

        for(opt<const CommandPool>::ref commandPool: objects.commandPool.graphics)
        {
            vkDestroyCommandPool(objects.device.handle, commandPool, ND_VULKAN_ALLOCATION_CALLBACKS);
        }

        for(opt<const CommandPool>::ref commandPool: objects.commandPool.transfer)
        {
            vkDestroyCommandPool(objects.device.handle, commandPool, ND_VULKAN_ALLOCATION_CALLBACKS);
        }

        for(opt<const CommandPool>::ref commandPool: objects.commandPool.compute)
        {
            vkDestroyCommandPool(objects.device.handle, commandPool, ND_VULKAN_ALLOCATION_CALLBACKS);
        }

        vkDestroyPipeline(objects.device.handle, objects.pipeline.mesh, ND_VULKAN_ALLOCATION_CALLBACKS);

        vkDestroyPipelineLayout(objects.device.handle, objects.pipelineLayout.mesh, ND_VULKAN_ALLOCATION_CALLBACKS);

        vkDestroyPipelineCache(objects.device.handle, objects.pipelineCache, ND_VULKAN_ALLOCATION_CALLBACKS);

        vkDestroyDescriptorSetLayout(objects.device.handle, objects.descriptorSetLayout.mesh, ND_VULKAN_ALLOCATION_CALLBACKS);

        vkDestroyDescriptorPool(objects.device.handle, objects.descriptorPool, ND_VULKAN_ALLOCATION_CALLBACKS);

        for(opt<const ShaderModule>::ref shaderModule: objects.shaderModules)
        {
            vkDestroyShaderModule(objects.device.handle, shaderModule.handle, ND_VULKAN_ALLOCATION_CALLBACKS);
        }

        for(opt<const Framebuffer>::ref swapchainFramebuffer: objects.swapchainFramebuffers)
        {
            vkDestroyFramebuffer(objects.device.handle, swapchainFramebuffer, ND_VULKAN_ALLOCATION_CALLBACKS);
        }

        for(opt<const ImageView>::ref swapchainImageView: objects.swapchainImageViews)
        {
            vkDestroyImageView(objects.device.handle, swapchainImageView, ND_VULKAN_ALLOCATION_CALLBACKS);
        }

        vkDestroyRenderPass(objects.device.handle, objects.renderPass, ND_VULKAN_ALLOCATION_CALLBACKS);
        vkDestroySwapchainKHR(objects.device.handle, objects.swapchain.handle, ND_VULKAN_ALLOCATION_CALLBACKS);
        vkDestroySurfaceKHR(objects.instance, objects.surface, ND_VULKAN_ALLOCATION_CALLBACKS);

        vkDestroyBuffer(objects.device.handle, objects.buffer.mesh.handle, ND_VULKAN_ALLOCATION_CALLBACKS);
        vkDestroyBuffer(objects.device.handle, objects.buffer.stage.handle, ND_VULKAN_ALLOCATION_CALLBACKS);

        vkFreeMemory(objects.device.handle, objects.device.memory.device.handle, ND_VULKAN_ALLOCATION_CALLBACKS);
        vkFreeMemory(objects.device.handle, objects.device.memory.host.handle, ND_VULKAN_ALLOCATION_CALLBACKS);

        vkDestroyDevice(objects.device.handle, ND_VULKAN_ALLOCATION_CALLBACKS);
        vkDestroyInstance(objects.instance, ND_VULKAN_ALLOCATION_CALLBACKS);
    }
} // namespace nd::src::graphics::vulkan
