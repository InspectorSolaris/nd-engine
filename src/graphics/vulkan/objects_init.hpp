#pragma once

#include "pch.hpp"
#include "tools.hpp"

#include "objects.hpp"
#include "objects_cfg.hpp"

#include "instance_init.hpp"
#include "device_init.hpp"
#include "buffer_init.hpp"
#include "surface_init.hpp"
#include "swapchain_init.hpp"
#include "render_pass_init.hpp"
#include "shader_module_init.hpp"
#include "descriptor_init.hpp"
#include "pipeline_init.hpp"
#include "command_init.hpp"

namespace nd::src::graphics::vulkan
{
    struct ObjectsInit final
    {
        using InstanceInit                   = rm_noexcept<decltype(createInstance)>;
        using PhysicalDeviceInit             = rm_noexcept<decltype(getPhysicalDevice)>;
        using DeviceInit                     = rm_noexcept<decltype(createDevice)>;
        using BufferObjectsInit              = rm_noexcept<decltype(createBufferObjects)>;
        using SurfaceInit                    = rm_noexcept<decltype(createSurface)>;
        using SwapchainInit                  = rm_noexcept<decltype(createSwapchain)>;
        using RenderPassInit                 = rm_noexcept<decltype(createRenderPass)>;
        using SwapchainImagesInit            = rm_noexcept<decltype(getSwapchainImages)>;
        using SwapchainImageViewsInit        = rm_noexcept<decltype(createSwapchainImageViews)>;
        using SwapchainFramebuffersInit      = rm_noexcept<decltype(createSwapchainFramebuffers)>;
        using DescriptorPoolInit             = rm_noexcept<decltype(createDescriptorPool)>;
        using DescriptorSetLayoutObjectsInit = rm_noexcept<decltype(createDescriptorSetLayoutObjects)>;
        using ShaderModulesInit              = rm_noexcept<decltype(createShaderModules)>;
        using PipelineCacheInit              = rm_noexcept<decltype(createPipelineCache)>;
        using PipelineLayoutObjectsInit      = rm_noexcept<decltype(createPipelineLayoutObjects)>;
        using PipelineObjectsInit            = rm_noexcept<decltype(createPipelineObjects)>;
        using CommandPoolObjectsInit         = rm_noexcept<decltype(createCommandPoolObjects)>;

        func<InstanceInit>                   instance;
        func<PhysicalDeviceInit>             physicalDevice;
        func<DeviceInit>                     device;
        func<BufferObjectsInit>              buffer;
        func<SurfaceInit>                    surface;
        func<SwapchainInit>                  swapchain;
        func<RenderPassInit>                 renderPass;
        func<SwapchainImagesInit>            swapchainImages;
        func<SwapchainImageViewsInit>        swapchainImageViews;
        func<SwapchainFramebuffersInit>      swapchainFramebuffers;
        func<DescriptorPoolInit>             descriptorPool;
        func<DescriptorSetLayoutObjectsInit> descriptorSetLayout;
        func<ShaderModulesInit>              shaderModules;
        func<PipelineCacheInit>              pipelineCache;
        func<PipelineLayoutObjectsInit>      pipelineLayout;
        func<PipelineObjectsInit>            pipeline;
        func<CommandPoolObjectsInit>         commandPool;
    };
} // namespace nd::src::graphics::vulkan
