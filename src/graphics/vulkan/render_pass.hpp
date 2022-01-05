#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    using AttachmentDescription = VkAttachmentDescription;
    using SubpassDependency     = VkSubpassDependency;

    struct SubpassDescription final
    {
        const std::vector<VkAttachmentReference> inputAttachments;
        const std::vector<VkAttachmentReference> colorAttachments;
        const std::vector<VkAttachmentReference> resolveAttachments;
        const std::vector<uint32_t>              preserveAttachments;

        const std::optional<VkAttachmentReference> depthStencilAttachment;

        const VkPipelineBindPoint pipelineBindPoint;

        const VkSubpassDescriptionFlags flags = {};
    };

    struct RenderPassConfiguration final
    {
        const std::vector<AttachmentDescription> attachments;
        const std::vector<SubpassDescription>    subpasses;
        const std::vector<SubpassDependency>     dependencies;

        const VkRenderPassCreateFlags flags = {};
        const void*                   next  = {};
    };

    using RenderPass = VkRenderPass;

    VkRenderPassBeginInfo
    getRenderPassBeginInfo(const VkRenderPass  renderPass,
                           const VkFramebuffer framebuffer,
                           const VkRect2D      renderArea,
                           const uint32_t      clearValueCount,
                           const VkClearValue* clearValues,
                           const void*         next = {}) noexcept;

    VkAttachmentDescription
    getRenderPassAttachment(const VkFormat                     format,
                            const VkSampleCountFlagBits        samples,
                            const VkAttachmentLoadOp           loadOp,
                            const VkAttachmentStoreOp          storeOp,
                            const VkAttachmentLoadOp           stencilLoadOp,
                            const VkAttachmentStoreOp          stencilStoreOp,
                            const VkImageLayout                initialLayout,
                            const VkImageLayout                finalLayout,
                            const VkAttachmentDescriptionFlags flags = {}) noexcept;

    VkSubpassDescription
    getRenderPassSubpass(const VkPipelineBindPoint       pipelineBindPoint,
                         const uint32_t                  inputAttachmentsCount,
                         const uint32_t                  colorAttachmentsCount,
                         const uint32_t                  preserveAttachmentsCount,
                         const VkAttachmentReference*    inputAttachments,
                         const VkAttachmentReference*    colorAttachments,
                         const VkAttachmentReference*    resolveAttachments,
                         const VkAttachmentReference*    depthStencilAttachments,
                         const uint32_t*                 preserveAttachments,
                         const VkSubpassDescriptionFlags flags = {}) noexcept;

    VkSubpassDependency
    getRenderPassDependency(const uint32_t             srcSubpass,
                            const uint32_t             dstSubpass,
                            const VkPipelineStageFlags srcStageMask,
                            const VkPipelineStageFlags dstStageMask,
                            const VkAccessFlags        srcAccessMask,
                            const VkAccessFlags        dstAccessMask,
                            const VkDependencyFlags    dependencyFlags) noexcept;

    VkRenderPassCreateInfo
    getRenderPassCreateInfo(const uint32_t                 attachmentsCount,
                            const uint32_t                 subpassesCount,
                            const uint32_t                 dependenciesCount,
                            const VkAttachmentDescription* attachments,
                            const VkSubpassDescription*    subpasses,
                            const VkSubpassDependency*     dependencies,
                            const VkRenderPassCreateFlags  flags = {},
                            const void*                    next  = {}) noexcept;

    VkRenderPass
    getRenderPassHandle(const VkRenderPassCreateInfo& createInfo, const VkDevice device);

    RenderPass
    getRenderPass(const RenderPassConfiguration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
