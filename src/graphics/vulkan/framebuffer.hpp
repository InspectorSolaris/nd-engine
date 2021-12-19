#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    struct FramebufferConfiguration final
    {
        const std::vector<VkImageView>& attachments;

        const VkRenderPass renderPass;

        const uint32_t width;
        const uint32_t height;
        const uint32_t layers;
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

    VkFramebufferCreateInfo
    getFramebufferCreateInfo(const FramebufferConfiguration& configuration);

    VkFramebuffer
    getFramebuffer(const VkFramebufferCreateInfo& createInfo, const VkDevice device);
} // namespace nd::src::graphics::vulkan
