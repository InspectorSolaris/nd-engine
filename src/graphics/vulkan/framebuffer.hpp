#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class Framebuffer final
    {
    public:
        using CreateInfo  = VkFramebufferCreateInfo;
        using Attachment  = VkImageView;
        using Attachments = std::vector<Attachment>;

        struct Configuration final
        {
            const uint32_t width;
            const uint32_t height;
            const uint32_t layers;
        };

        Framebuffer() noexcept;
        Framebuffer(const VkDevice device, const CreateInfo& createInfo);

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

    Framebuffer::CreateInfo
    getFramebufferCreateInfo(const VkRenderPass renderPass,
                             const uint32_t     attachmentsCount,
                             const VkImageView* attachments,
                             const uint32_t     width,
                             const uint32_t     height,
                             const uint32_t     layers) noexcept;

    Framebuffer
    getFramebuffer(const Framebuffer::Configuration& configuration,
                   const Framebuffer::Attachments&   attachments,
                   const VkDevice                    device,
                   const VkRenderPass                renderPass);
} // namespace nd::src::graphics::vulkan
