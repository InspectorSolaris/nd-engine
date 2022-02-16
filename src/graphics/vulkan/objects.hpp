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
    using CommandPool         = VkCommandPool;
    using CommandBuffer       = VkCommandBuffer;
    using Semaphore           = VkSemaphore;
    using Fence               = VkFence;

    // -------------- SS --------------
    // --------------------------------
    // ------------ DEVICE ------------
    // --------------------------------
    // -------------- SS --------------

    struct DeviceMemory final
    {
        VkDeviceMemory handle;

        u8 typeIndex;
        u8 heapIndex;
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

    // -------------- EE --------------
    // --------------------------------
    // ------------ DEVICE ------------
    // --------------------------------
    // -------------- EE --------------



    // --------------- S ---------------
    // ---------------------------------
    // ------------ BUFFERS ------------
    // ---------------------------------
    // --------------- S ---------------

    struct Buffer final
    {
        VkDeviceSize offset;

        VkBuffer handle;
    };

    struct BufferObjects final
    {
        Buffer mesh;
        Buffer stage;
    };

    // --------------- E ---------------
    // ---------------------------------
    // ------------ BUFFERS ------------
    // ---------------------------------
    // --------------- E ---------------



    // -------------- SS --------------
    // --------------------------------
    // ------------ SCREEN ------------
    // --------------------------------
    // -------------- SS --------------

    struct Swapchain final
    {
        QueueFamily queueFamily;

        VkSwapchainKHR handle;

        u16 width;
        u16 height;
    };

    // -------------- EE --------------
    // --------------------------------
    // ------------ SCREEN ------------
    // --------------------------------
    // -------------- EE --------------



    // ----------------- S -----------------
    // -------------------------------------
    // ------------ DESCRIPTORS ------------
    // -------------------------------------
    // ----------------- S -----------------

    struct DescriptorSetLayoutObjects final
    {
        DescriptorSetLayout mesh;
    };

    // ----------------- E -----------------
    // -------------------------------------
    // ------------ DESCRIPTORS ------------
    // -------------------------------------
    // ----------------- E -----------------



    // ---------------- S ----------------
    // -----------------------------------
    // ------------ PIPELINES ------------
    // -----------------------------------
    // ---------------- S ----------------

    struct ShaderModule final
    {
        VkShaderStageFlagBits stage;

        VkShaderModule handle;
    };

    struct PipelineLayoutObjects final
    {
        PipelineLayout mesh;
    };

    struct PipelineObjects final
    {
        Pipeline mesh;
    };

    // ---------------- E ----------------
    // -----------------------------------
    // ------------ PIPELINES ------------
    // -----------------------------------
    // ---------------- E ----------------



    // --------------- SS ---------------
    // ----------------------------------
    // ------------ COMMANDS ------------
    // ----------------------------------
    // --------------- SS ---------------

    struct CommandPoolObjects final
    {
        vec<CommandPool> graphics;
        vec<CommandPool> transfer;
        vec<CommandPool> compute;
    };

    // --------------- EE ---------------
    // ----------------------------------
    // ------------ COMMANDS ------------
    // ----------------------------------
    // --------------- EE ---------------

    struct VulkanObjects final
    {
        Device device;

        BufferObjects buffer;

        vec<Image>        swapchainImages;
        vec<ImageView>    swapchainImageViews;
        vec<Framebuffer>  swapchainFramebuffers;
        vec<ShaderModule> shaderModules;

        vec<Semaphore> semaphores;
        vec<Fence>     fences;

        CommandPoolObjects         commandPool;
        DescriptorSetLayoutObjects descriptorSetLayout;
        PipelineLayoutObjects      pipelineLayout;
        PipelineObjects            pipeline;

        Swapchain      swapchain;
        Instance       instance;
        PhysicalDevice physicalDevice;
        Surface        surface;
        RenderPass     renderPass;
        DescriptorPool descriptorPool;
        PipelineCache  pipelineCache;
    };
} // namespace nd::src::graphics::vulkan
