#pragma once

#include "pch.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using Instance            = VkInstance;
    using PhysicalDevice      = VkPhysicalDevice;
    using Surface             = VkSurfaceKHR;
    using RenderPass          = VkRenderPass;
    using Image               = VkImage;
    using ImageView           = VkImageView;
    using Framebuffer         = VkFramebuffer;
    using DescriptorPool      = VkDescriptorPool;
    using DescriptorSetLayout = VkDescriptorSetLayout;
    using DescriptorSet       = VkDescriptorSet;
    using PipelineCache       = VkPipelineCache;
    using PipelineLayout      = VkPipelineLayout;
    using Pipeline            = VkPipeline;

    struct DeviceMemory final
    {
        VkDeviceSize size;

        VkDeviceMemory handle;
    };

    struct QueueFamily final
    {
        VkQueueFlags queueFlags;
        u16          queueCount;
        u16          index;
    };

    struct DeviceMemoryObjects final
    {
        DeviceMemory device;
        DeviceMemory host;
    };

    struct QueueFamilyObjects final
    {
        QueueFamily graphics;
        QueueFamily transfer;
        QueueFamily compute;
    };

    struct Device final
    {
        DeviceMemoryObjects memory;
        QueueFamilyObjects  queueFamily;

        VkDevice handle;
    };

    struct Swapchain final
    {
        QueueFamily queueFamily;

        VkSwapchainKHR handle;
    };

    struct ShaderModule final
    {
        VkShaderStageFlagBits stage;

        VkShaderModule handle;
    };

    struct DescriptorSetLayoutObjects final
    {
        DescriptorSetLayout mesh;
    };

    struct PipelineLayoutObjects final
    {
        PipelineLayout mesh;
    };

    struct PipelineObjects final
    {
        Pipeline mesh;
    };

    struct BufferRange final
    {
        VkDeviceSize offset;
        VkDeviceSize size;
    };

    struct BufferMesh final
    {
        BufferRange vertex;
        BufferRange index;
        BufferRange uniform;

        VkDeviceMemory memory;
        VkBuffer       handle;
    };

    struct BufferStage final
    {
        BufferRange range;

        VkDeviceMemory memory;
        VkBuffer       handle;
    };

    struct BufferObjects final
    {
        BufferMesh  mesh;
        BufferStage stage;
    };

    struct VulkanObjects final
    {
        Device device;

        vec<Image>        swapchainImages;
        vec<ImageView>    swapchainImageViews;
        vec<Framebuffer>  swapchainFramebuffers;
        vec<ShaderModule> shaderModules;

        DescriptorSetLayoutObjects descriptorSetLayoutObjects;
        PipelineLayoutObjects      pipelineLayoutObjects;
        PipelineObjects            pipelineObjects;

        Swapchain      swapchain;
        Instance       instance;
        PhysicalDevice physicalDevice;
        Surface        surface;
        RenderPass     renderPass;
        DescriptorPool descriptorPool;
        PipelineCache  pipelineCache;
    };
} // namespace nd::src::graphics::vulkan
