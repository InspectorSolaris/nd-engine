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
#include "framebuffer.hpp"
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
            const std::function<VkSurfaceKHR(const VkInstance instance)>& getSurface;

            const std::string& applicationName;
            const std::string& engineName;

            const std::vector<std::string>& layers;
            const std::vector<std::string>& extensions;

            const uint32_t width {};
            const uint32_t height {};
        };

        Context(Instance&&                  instance,
                Device&&                    device,
                Surface&&                   surface,
                Swapchain&&                 swapchain,
                RenderPass&&                renderPass,
                std::vector<VkImage>&&      swapchainImages,
                std::vector<ImageView>&&    swapchainImageViews,
                std::vector<Framebuffer>&&  swapchainFramebuffers,
                std::vector<ShaderModule>&& shaderModules,
                DescriptorPool&&            descriptorPool,
                DescriptorSetLayout&&       descriptorSetLayout,
                DescriptorSet&&             descriptorSet);

        Context(const Context& vulkanContext) = delete;
        Context(Context&& vulkanContext)      = delete;

        ~Context();

        Context&
        operator=(const Context& vulkanContext) = delete;
        Context&
        operator=(Context&& vulkanContext) = delete;

    private:
        Instance                  instance_ {};
        Device                    device_ {};
        Surface                   surface_ {};
        Swapchain                 swapchain_ {};
        RenderPass                renderPass_ {};
        std::vector<VkImage>      swapchainImages_ {};
        std::vector<ImageView>    swapchainImageViews_ {};
        std::vector<Framebuffer>  swapchainFramebuffers_ {};
        std::vector<ShaderModule> shaderModules_ {};
        DescriptorPool            descriptorPool_ {};
        DescriptorSetLayout       descriptorSetLayout_ {};
        DescriptorSet             descriptorSet_ {};
    };

    Context
    getContext(const Context::Configuration& configuration);
} // namespace nd::src::graphics::vulkan
