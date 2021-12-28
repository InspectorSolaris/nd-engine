#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    struct FramebufferConfiguration final
    {
        const std::vector<VkImageView> attachments;

        const VkRenderPass renderPass;

        const uint32_t width;
        const uint32_t height;
        const uint32_t layers;

        const VkFramebufferCreateFlags flags = {};
        const void*                    next  = {};
    };

    struct Framebuffer final
    {
        const VkFramebuffer handle;
    };

    VkFramebufferCreateInfo
    getFramebufferCreateInfo(const VkRenderPass             renderPass,
                             const uint32_t                 attachmentsCount,
                             const VkImageView*             attachments,
                             const uint32_t                 width,
                             const uint32_t                 height,
                             const uint32_t                 layers,
                             const VkFramebufferCreateFlags flags = {},
                             const void*                    next  = {}) noexcept;

    VkFramebuffer
    getFramebufferHandle(const VkFramebufferCreateInfo& createInfo, const VkDevice device);

    Framebuffer
    getFramebuffer(const FramebufferConfiguration& configuration, const VkDevice device);

    std::vector<Framebuffer>
    getFramebuffers(const std::vector<FramebufferConfiguration>& configurations, const VkDevice device);
} // namespace nd::src::graphics::vulkan
