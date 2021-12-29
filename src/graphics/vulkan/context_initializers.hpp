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

        using DescriptorSetLayout = DescriptorSetLayout(const DescriptorSetLayoutConfiguration&, const VkDevice);

        using DescriptorSets = DescriptorSets(const DescriptorSetsConfiguration&, const VkDevice);

        using PipelineLayout = PipelineLayout(const PipelineLayoutConfiguration&, const VkDevice);

        using Pipelines = Pipelines(const std::vector<PipelineConfiguration>&, const VkDevice);

        using CommandPools = std::vector<CommandPool>(const std::vector<CommandPoolConfiguration>&, const VkDevice);

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
        const std::function<DescriptorSetLayout>   getDescriptorSetLayout;
        const std::function<DescriptorSets>        getDescriptorSets;
        const std::function<PipelineLayout>        getPipelineLayout;
        const std::function<Pipelines>             getPipelines;
        const std::function<CommandPools>          getCommandPools;
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
        add(decltype(Type::getDescriptorSetLayout)& initializer) noexcept
        {
            getDescriptorSetLayout = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getDescriptorSets)& initializer) noexcept
        {
            getDescriptorSets = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getPipelineLayout)& initializer) noexcept
        {
            getPipelineLayout = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getPipelines)& initializer) noexcept
        {
            getPipelines = initializer;

            return *this;
        }

        Builder&
        add(decltype(Type::getCommandPools)& initializer) noexcept
        {
            getCommandPools = initializer;

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
        operator<<(decltype(Type::getDescriptorSetLayout)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getDescriptorSets)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getPipelineLayout)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getPipelines)& initializer) noexcept
        {
            return add(initializer);
        }

        Builder&
        operator<<(decltype(Type::getCommandPools)& initializer) noexcept
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
        std::remove_cv_t<decltype(Type::getDescriptorSetLayout)>   getDescriptorSetLayout;
        std::remove_cv_t<decltype(Type::getDescriptorSets)>        getDescriptorSets;
        std::remove_cv_t<decltype(Type::getPipelineLayout)>        getPipelineLayout;
        std::remove_cv_t<decltype(Type::getPipelines)>             getPipelines;
        std::remove_cv_t<decltype(Type::getCommandPools)>          getCommandPools;
    };

    extern VulkanContextInitializersBuilder initializersBuilder;
} // namespace nd::src::graphics::vulkan
