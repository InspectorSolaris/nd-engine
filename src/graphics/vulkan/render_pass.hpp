#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class RenderPass final
    {
    public:
        using CreateInfo = VkRenderPassCreateInfo;

        struct Configuration final
        {
            const std::vector<VkAttachmentDescription>& attachments;
            const std::vector<VkSubpassDescription>&    subpasses;
            const std::vector<VkSubpassDependency>&     dependencies;
        };

        RenderPass() noexcept;
        RenderPass(const VkDevice device, const CreateInfo& createInfo);

        RenderPass(const RenderPass& renderPass) = delete;
        RenderPass(RenderPass&& renderPass) noexcept;

        RenderPass&
        operator=(const RenderPass& renderPass) = delete;
        RenderPass&
        operator=(RenderPass&& renderPass) noexcept;

        ~RenderPass();

        constexpr VkRenderPass
        get() const noexcept;

    private:
        VkDevice     device_ {VK_NULL_HANDLE};
        VkRenderPass renderPass_ {VK_NULL_HANDLE};
    };

    constexpr VkRenderPass
    RenderPass::get() const noexcept
    {
        return renderPass_;
    }

    VkAttachmentDescription
    getRenderPassAttachment(const VkFormat              format,
                            const VkSampleCountFlagBits samples,
                            const VkAttachmentLoadOp    loadOp,
                            const VkAttachmentStoreOp   storeOp,
                            const VkAttachmentLoadOp    stencilLoadOp,
                            const VkAttachmentStoreOp   stencilStoreOp,
                            const VkImageLayout         initialLayout,
                            const VkImageLayout         finalLayout) noexcept;

    VkSubpassDescription
    getRenderPassSubpass(const VkPipelineBindPoint    pipelineBindPoint,
                         const uint32_t               inputAttachmentsCount,
                         const uint32_t               colorAttachmentsCount,
                         const uint32_t               preserveAttachmentsCount,
                         const VkAttachmentReference* inputAttachments,
                         const VkAttachmentReference* colorAttachments,
                         const VkAttachmentReference* resolveAttachments,
                         const VkAttachmentReference* depthStencilAttachments,
                         const uint32_t*              preserveAttachments) noexcept;

    VkSubpassDependency
    getRenderPassDependency(const uint32_t             srcSubpass,
                            const uint32_t             dstSubpass,
                            const VkPipelineStageFlags srcStageMask,
                            const VkPipelineStageFlags dstStageMask,
                            const VkAccessFlags        srcAccessMask,
                            const VkAccessFlags        dstAccessMask,
                            const VkDependencyFlags    dependencyFlags) noexcept;

    RenderPass::CreateInfo
    getRenderPassCreateInfo(const uint32_t                 attachmentsCount,
                            const uint32_t                 subpassesCount,
                            const uint32_t                 dependenciesCount,
                            const VkAttachmentDescription* attachments,
                            const VkSubpassDescription*    subpasses,
                            const VkSubpassDependency*     dependencies) noexcept;

    RenderPass
    getRenderPass(const RenderPass::Configuration& configuration, const VkDevice device);
} // namespace nd::src::graphics::vulkan
