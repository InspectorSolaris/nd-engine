#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class Framebuffer final
    {
    public:
        struct Configuration final
        {
            const std::vector<VkImageView>& attachments;

            const VkRenderPass renderPass;

            const uint32_t width;
            const uint32_t height;
            const uint32_t layers;
        };

        Framebuffer() noexcept;
        Framebuffer(const VkDevice device, const VkFramebufferCreateInfo& createInfo);

        Framebuffer(const Framebuffer& framebuffer) = delete;
        Framebuffer(Framebuffer&& framebuffer) noexcept;

        Framebuffer&
        operator=(const Framebuffer& framebuffer) = delete;
        Framebuffer&
        operator=(Framebuffer&& framebuffer) noexcept;

        ~Framebuffer();

        constexpr VkFramebuffer
        get() const noexcept;

    private:
        VkDevice      device_ {VK_NULL_HANDLE};
        VkFramebuffer framebuffer_ {VK_NULL_HANDLE};
    };

    constexpr VkFramebuffer
    Framebuffer::get() const noexcept
    {
        return framebuffer_;
    }

    VkFramebufferCreateInfo
    getFramebufferCreateInfo(const VkRenderPass             renderPass,
                             const uint32_t                 attachmentsCount,
                             const VkImageView*             attachments,
                             const uint32_t                 width,
                             const uint32_t                 height,
                             const uint32_t                 layers,
                             const VkFramebufferCreateFlags flags = {},
                             const void*                    next  = {}) noexcept;

    Framebuffer
    getFramebuffer(const Framebuffer::Configuration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
