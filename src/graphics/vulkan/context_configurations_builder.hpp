#pragma once

#include "pch.hpp"
#include "shared.hpp"

#include "context_configurations.hpp"

#define ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(field) \
    Builder& add(decltype(Type::field)& configuration) noexcept \
    {                                                           \
        field = configuration;                                  \
        return *this;                                           \
    }

#define ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(field)               \
    Builder& operator<<(decltype(Type::field)& configuration) noexcept \
    {                                                                  \
        return add(configuration);                                     \
    }

namespace nd::src::graphics::vulkan
{
    class VulkanConfigurationsBuilder final
    {
    public:
        using Type    = VulkanConfigurations;
        using Builder = VulkanConfigurationsBuilder;

        Type
        build() const;

        operator Type() const noexcept
        {
            return build();
        }

        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getInstance);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getPhysicalDevice);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getDevice);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getSwapchain);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getRenderPass);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getSwapchainImageViews);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getSwapchainFramebuffers);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getShaderModules);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getDescriptorPool);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getDescriptorSetLayouts);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getDescriptorSet);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getPipelineLayouts);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getGraphicsPipelines);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getCommandPools);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getCommandBuffers);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getBuffers);
        ND_DEFINE_CONFIGURATIONS_BUILDER_MEMBER_FUNCTION(getBufferMemories);

        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getInstance);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getPhysicalDevice);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getDevice);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getSwapchain);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getRenderPass);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getSwapchainImageViews);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getSwapchainFramebuffers);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getShaderModules);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getDescriptorPool);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getDescriptorSetLayouts);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getDescriptorSet);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getPipelineLayouts);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getGraphicsPipelines);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getCommandPools);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getCommandBuffers);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getBuffers);
        ND_DEFINE_CONFIGURATIONS_BUILDER_OPERATOR(getBufferMemories);

    private:
        std::remove_cv_t<decltype(Type::getInstance)>              getInstance;
        std::remove_cv_t<decltype(Type::getPhysicalDevice)>        getPhysicalDevice;
        std::remove_cv_t<decltype(Type::getDevice)>                getDevice;
        std::remove_cv_t<decltype(Type::getSwapchain)>             getSwapchain;
        std::remove_cv_t<decltype(Type::getRenderPass)>            getRenderPass;
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
        std::remove_cv_t<decltype(Type::getBuffers)>               getBuffers;
        std::remove_cv_t<decltype(Type::getBufferMemories)>        getBufferMemories;
    };

    extern VulkanConfigurationsBuilder configurationsBuilder;
} // namespace nd::src::graphics::vulkan
