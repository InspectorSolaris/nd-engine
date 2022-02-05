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
        auto       instance    = init.instance(instanceCfg);

        const auto physicalDeviceCfg = cfg.physicalDevice();
        auto       physicalDevice    = init.physicalDevice(physicalDeviceCfg, instance);

        const auto deviceCfg = cfg.device(physicalDeviceCfg);
        auto       device    = init.device(deviceCfg, physicalDevice);

        auto surface = init.surface(instance);

        const auto swapchainCfg = cfg.swapchain(dependency, physicalDevice, device, surface);
        auto       swapchain    = init.swapchain(swapchainCfg, device.handle);

        const auto renderPassCfg = cfg.renderPass(swapchainCfg);
        auto       renderPass    = init.renderPass(renderPassCfg, device.handle);

        auto swapchainImages = init.swapchainImages(device.handle, swapchain.handle);

        const auto swapchainImageViewCfg = cfg.swapchainImageView(swapchainCfg);
        auto       swapchainImageViews   = init.swapchainImageViews(swapchainImageViewCfg, device.handle, swapchainImages);

        const auto swapchainFramebufferCfg = cfg.swapchainFramebuffer(swapchainCfg, renderPass);
        auto       swapchainFramebuffers   = init.swapchainFramebuffers(swapchainFramebufferCfg, device.handle, swapchainImageViews);

        const auto shaderModuleCfgs = cfg.shaderModules();
        auto       shaderModules    = init.shaderModules(shaderModuleCfgs, device.handle);

        const auto pipelineCacheCfg = cfg.pipelineCache();
        auto       pipelineCache    = init.pipelineCache(pipelineCacheCfg, device.handle);

        return {.device                = device,
                .swapchainImages       = std::move(swapchainImages),
                .swapchainImageViews   = std::move(swapchainImageViews),
                .swapchainFramebuffers = std::move(swapchainFramebuffers),
                .shaderModules         = std::move(shaderModules),
                .swapchain             = swapchain,
                .instance              = instance,
                .physicalDevice        = physicalDevice,
                .surface               = surface,
                .renderPass            = renderPass,
                .pipelineCache         = pipelineCache};
    }

    void
    destroyVulkanObjects(opt<const VulkanObjects>::ref objects) noexcept
    {
        ND_SET_SCOPE();

        vkFreeMemory(objects.device.handle, objects.device.memoryPool.device.handle, ND_VULKAN_ALLOCATION_CALLBACKS);
        vkFreeMemory(objects.device.handle, objects.device.memoryPool.host.handle, ND_VULKAN_ALLOCATION_CALLBACKS);

        vkDestroyPipelineCache(objects.device.handle, objects.pipelineCache, ND_VULKAN_ALLOCATION_CALLBACKS);

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
        vkDestroyDevice(objects.device.handle, ND_VULKAN_ALLOCATION_CALLBACKS);
        vkDestroyInstance(objects.instance, ND_VULKAN_ALLOCATION_CALLBACKS);
    }
} // namespace nd::src::graphics::vulkan
