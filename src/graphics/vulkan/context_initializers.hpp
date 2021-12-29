#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct VulkanContextInitializers final
    {
        using Instance = Instance(const InstanceConfiguration&);

        using PhysicalDevice = PhysicalDevice(const PhysicalDeviceConfiguration&, const VkInstance);

        using Device = Device(const DeviceConfiguration&, const VkPhysicalDevice);

        using Surface = Surface(const VkInstance);

        using Swapchain = Swapchain(const SwapchainConfiguration&, const VkDevice);

        using RenderPass = RenderPass(const RenderPassConfiguration&, const VkDevice);

        using SwapchainImages = std::vector<Image>(const VkSwapchainKHR, const VkDevice);

        using SwapchainImageViews = std::vector<ImageView>(const std::vector<ImageViewConfiguration>&, const VkDevice);

        using SwapchainFramebuffers = std::vector<Framebuffer>(const std::vector<FramebufferConfiguration>&, const VkDevice);

        using ShaderModules = std::vector<ShaderModule>(const std::vector<ShaderModuleConfiguration>&, const VkDevice);

        using DescriptorPool = DescriptorPool(const DescriptorPoolConfiguration&, const VkDevice);

        using DescriptorSetLayouts = std::vector<DescriptorSetLayout>(const std::vector<DescriptorSetLayoutConfiguration>&, const VkDevice);

        using DescriptorSet = DescriptorSet(const DescriptorSetConfiguration&, const VkDevice);

        using PipelineLayouts = std::vector<PipelineLayout>(const std::vector<PipelineLayoutConfiguration>&, const VkDevice);

        using GraphicsPipelines = Pipelines(const std::vector<GraphicsPipelineConfiguration>&, const VkDevice);

        using CommandPools = std::vector<CommandPool>(const std::vector<CommandPoolConfiguration>&, const VkDevice);

        using CommandBuffers = std::vector<CommandBuffer>(const std::vector<CommandBufferConfiguration>&, const VkDevice);

        const std::function<Instance>              getInstance;
        const std::function<PhysicalDevice>        getPhysicalDevice;
        const std::function<Device>                getDevice;
        const std::function<Surface>               getSurface;
        const std::function<Swapchain>             getSwapchain;
        const std::function<RenderPass>            getRenderPass;
        const std::function<SwapchainImages>       getSwapchainImages;
        const std::function<SwapchainImageViews>   getSwapchainImageViews;
        const std::function<SwapchainFramebuffers> getSwapchainFramebuffers;
        const std::function<ShaderModules>         getShaderModules;
        const std::function<DescriptorPool>        getDescriptorPool;
        const std::function<DescriptorSetLayouts>  getDescriptorSetLayouts;
        const std::function<DescriptorSet>         getDescriptorSet;
        const std::function<PipelineLayouts>       getPipelineLayouts;
        const std::function<GraphicsPipelines>     getGraphicsPipelines;
        const std::function<CommandPools>          getCommandPools;
        const std::function<CommandBuffers>        getCommandBuffers;
    };

    class VulkanContextInitializersBuilder final
    {
    public:
        using Type    = VulkanContextInitializers;
        using Builder = VulkanContextInitializersBuilder;

        Type
        build() const;

        operator Type() const noexcept
        {
            return build();
        }

        Builder&
        add(decltype(Type::getInstance)& initializer) noexcept
        {
            getInstance = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getPhysicalDevice)& initializer) noexcept
        {
            getPhysicalDevice = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getDevice)& initializer) noexcept
        {
            getDevice = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getSurface)& initializer) noexcept
        {
            getSurface = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getSwapchain)& initializer) noexcept
        {
            getSwapchain = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getRenderPass)& initializer) noexcept
        {
            getRenderPass = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getSwapchainImages)& initializer) noexcept
        {
            getSwapchainImages = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getSwapchainImageViews)& initializer) noexcept
        {
            getSwapchainImageViews = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getSwapchainFramebuffers)& initializer) noexcept
        {
            getSwapchainFramebuffers = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getShaderModules)& initializer) noexcept
        {
            getShaderModules = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getDescriptorPool)& initializer) noexcept
        {
            getDescriptorPool = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getDescriptorSetLayouts)& initializer) noexcept
        {
            getDescriptorSetLayouts = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getDescriptorSet)& initializer) noexcept
        {
            getDescriptorSet = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getPipelineLayouts)& initializer) noexcept
        {
            getPipelineLayouts = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getGraphicsPipelines)& initializer) noexcept
        {
            getGraphicsPipelines = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getCommandPools)& initializer) noexcept
        {
            getCommandPools = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getCommandBuffers)& initializer) noexcept
        {
            getCommandBuffers = initializer;

            return *this;
        }

        Builder&
        operator<<(decltype(Type::getInstance)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getPhysicalDevice)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getDevice)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getSurface)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getSwapchain)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getRenderPass)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getSwapchainImages)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getSwapchainImageViews)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getSwapchainFramebuffers)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getShaderModules)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getDescriptorPool)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getDescriptorSetLayouts)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getDescriptorSet)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getPipelineLayouts)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getGraphicsPipelines)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getCommandPools)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getCommandBuffers)& initializer) noexcept
        {
            return add(initializer);
        }

    private:
        std::remove_cv_t<decltype(Type::getInstance)>              getInstance;
        std::remove_cv_t<decltype(Type::getPhysicalDevice)>        getPhysicalDevice;
        std::remove_cv_t<decltype(Type::getDevice)>                getDevice;
        std::remove_cv_t<decltype(Type::getSurface)>               getSurface;
        std::remove_cv_t<decltype(Type::getSwapchain)>             getSwapchain;
        std::remove_cv_t<decltype(Type::getRenderPass)>            getRenderPass;
        std::remove_cv_t<decltype(Type::getSwapchainImages)>       getSwapchainImages;
        std::remove_cv_t<decltype(Type::getSwapchainImageViews)>   getSwapchainImageViews;
        std::remove_cv_t<decltype(Type::getSwapchainFramebuffers)> getSwapchainFramebuffers;
        std::remove_cv_t<decltype(Type::getShaderModules)>         getShaderModules;
        std::remove_cv_t<decltype(Type::getDescriptorPool)>        getDescriptorPool;
        std::remove_cv_t<decltype(Type::getDescriptorSetLayouts)>  getDescriptorSetLayouts;
        std::remove_cv_t<decltype(Type::getDescriptorSet)>         getDescriptorSet;
        std::remove_cv_t<decltype(Type::getPipelineLayouts)>       getPipelineLayouts;
        std::remove_cv_t<decltype(Type::getGraphicsPipelines)>     getGraphicsPipelines;
        std::remove_cv_t<decltype(Type::getCommandPools)>          getCommandPools;
        std::remove_cv_t<decltype(Type::getCommandBuffers)>        getCommandBuffers;
    };

    extern VulkanContextInitializersBuilder initializersBuilder;
} // namespace nd::src::graphics::vulkan
