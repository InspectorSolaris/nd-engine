#pragma once

#include "pch.hpp"
#include "shared.hpp"

#include "instance.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "swapchain.hpp"
#include "descriptor_pool.hpp"
#include "descriptor_set_layout.hpp"
#include "descriptor_set.hpp"
#include "render_pass.hpp"
#include "shader_module.hpp"
#include "pipeline_layout.hpp"
#include "pipeline.hpp"
#include "image.hpp"
#include "image_view.hpp"
#include "buffer.hpp"
#include "buffer_view.hpp"
#include "command_pool.hpp"
#include "command_buffer.hpp"

namespace nd::src::graphics::vulkan
{
    class Context final
    {
    public:
        struct Configuration final
        {
            const std::function<VkSurfaceKHR(const VkInstance instance)> getSurface;

            const std::vector<std::string> layers;
            const std::vector<std::string> extensions;

            const uint32_t width;
            const uint32_t height;
        };

        Context(const Configuration& configuration);

        Context(const Context& vulkanContext) = delete;
        Context(Context&& vulkanContext)      = delete;

        ~Context();

        Context&
        operator=(const Context& vulkanContext) = delete;
        Context&
        operator=(Context&& vulkanContext) = delete;

    private:
        Instance                instance_ {};
        Device                  device_ {};
        Surface                 surface_ {};
        RenderPass              renderPass_ {};
        Swapchain               swapchain_ {};
        Swapchain::Images       swapchainImages_ {};
        Swapchain::ImageViews   swapchainImageViews_ {};
        Swapchain::Framebuffers swapchainFramebuffers_ {};
    };
} // namespace nd::src::graphics::vulkan
