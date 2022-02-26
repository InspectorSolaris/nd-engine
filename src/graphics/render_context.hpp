#pragma once

#include "pch.hpp"
#include "tools.hpp"

// nd::src::graphics::vulkan

#include "objects_complete.hpp"

namespace nd::src::graphics
{
    struct QueueObjects final
    {
        vec<vulkan::Queue> graphics;
        vec<vulkan::Queue> transfer;
        vec<vulkan::Queue> compute;
        vec<vulkan::Queue> swapchain;
    };

    struct CommandBufferCfg final
    {
        u16 graphicsCount;
        u16 transferCount;
        u16 computeCount;
    };

    struct CommandBufferObjects final
    {
        vec<vulkan::CommandBuffer> graphics;
        vec<vulkan::CommandBuffer> transfer;
        vec<vulkan::CommandBuffer> compute;
    };

    struct DescriptorSetObjects final
    {
        vec<vulkan::DescriptorSet> mesh;
    };

    struct SemaphoreObjects final
    {
        vec<vulkan::Semaphore> acquired;
        vec<vulkan::Semaphore> graphics;
        vec<vulkan::Semaphore> transfer;
        vec<vulkan::Semaphore> compute;
    };

    struct FenceObjects final
    {
        vec<vulkan::Fence> rendered;
    };

    struct CommandBufferView final
    {
        span<const vulkan::CommandBuffer> graphics;
        span<const vulkan::CommandBuffer> transfer;
        span<const vulkan::CommandBuffer> compute;
    };

    struct DescriptorSetView final
    {
        vulkan::DescriptorSet mesh;
    };

    struct SemaphoreView final
    {
        vulkan::Semaphore graphics;
        vulkan::Semaphore transfer;
        vulkan::Semaphore compute;
    };

    struct FenceView final
    {
        vulkan::Fence rendered;
    };

    struct RenderContext final
    {
        struct Frame final
        {
            CommandBufferView commandBuffer;
            SemaphoreView     semaphore;
            FenceView         fence;
            DescriptorSetView descriptorSet;
        };

        SemaphoreObjects semaphore;

        QueueObjects         queue;
        CommandBufferObjects commandBuffer;
        DescriptorSetObjects descriptorSet;

        FenceObjects fence;
    };

    RenderContext
    getRenderContext(vulkan::Objects&, const CommandBufferCfg, const u16, const u16) noexcept(ND_VK_ASSERT_NOTHROW);

    RenderContext::Frame
    getRenderContextFrame(const RenderContext&, const CommandBufferCfg, const u16, const u16, const u16) noexcept;
} // namespace nd::src::graphics
