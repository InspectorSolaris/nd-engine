#include "render_pass.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VkRenderPassBeginInfo
    getRenderPassBeginInfo(const VkRenderPass  renderPass,
                           const VkFramebuffer framebuffer,
                           const VkRect2D      renderArea,
                           const uint32_t      clearValueCount,
                           const VkClearValue* clearValues,
                           const void*         next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO, // sType;
            next,                                     // pNext;
            renderPass,                               // renderPass;
            framebuffer,                              // framebuffer;
            renderArea,                               // renderArea;
            clearValueCount,                          // clearValueCount;
            clearValues                               // pClearValues;
        };
    }

    VkAttachmentDescription
    getRenderPassAttachment(const VkFormat                     format,
                            const VkSampleCountFlagBits        samples,
                            const VkAttachmentLoadOp           loadOp,
                            const VkAttachmentStoreOp          storeOp,
                            const VkAttachmentLoadOp           stencilLoadOp,
                            const VkAttachmentStoreOp          stencilStoreOp,
                            const VkImageLayout                initialLayout,
                            const VkImageLayout                finalLayout,
                            const VkAttachmentDescriptionFlags flags) noexcept
    {
        ND_SET_SCOPE();

        return {
            flags,          // flags;
            format,         // format;
            samples,        // samples;
            loadOp,         // loadOp;
            storeOp,        // storeOp;
            stencilLoadOp,  // stencilLoadOp;
            stencilStoreOp, // stencilStoreOp;
            initialLayout,  // initialLayout;
            finalLayout     // finalLayout;
        };
    }

    VkSubpassDescription
    getRenderPassSubpass(const VkPipelineBindPoint       pipelineBindPoint,
                         const uint32_t                  inputAttachmentsCount,
                         const uint32_t                  colorAttachmentsCount,
                         const uint32_t                  preserveAttachmentsCount,
                         const VkAttachmentReference*    inputAttachments,
                         const VkAttachmentReference*    colorAttachments,
                         const VkAttachmentReference*    resolveAttachments,
                         const VkAttachmentReference*    depthStencilAttachment,
                         const uint32_t*                 preserveAttachments,
                         const VkSubpassDescriptionFlags flags) noexcept
    {
        ND_SET_SCOPE();

        return {
            flags,                    // flags;
            pipelineBindPoint,        // pipelineBindPoint;
            inputAttachmentsCount,    // inputAttachmentCount;
            inputAttachments,         // pInputAttachments;
            colorAttachmentsCount,    // colorAttachmentCount;
            colorAttachments,         // pColorAttachments;
            resolveAttachments,       // pResolveAttachments;
            depthStencilAttachment,   // pDepthStencilAttachment;
            preserveAttachmentsCount, // preserveAttachmentCount;
            preserveAttachments       // pPreserveAttachments;
        };
    }

    VkSubpassDependency
    getRenderPassDependency(const uint32_t             srcSubpass,
                            const uint32_t             dstSubpass,
                            const VkPipelineStageFlags srcStageMask,
                            const VkPipelineStageFlags dstStageMask,
                            const VkAccessFlags        srcAccessMask,
                            const VkAccessFlags        dstAccessMask,
                            const VkDependencyFlags    dependencyFlags) noexcept
    {
        ND_SET_SCOPE();

        return {
            srcSubpass,     // srcSubpass;
            dstSubpass,     // dstSubpass;
            srcStageMask,   // srcStageMask;
            dstStageMask,   // dstStageMask;
            srcAccessMask,  // srcAccessMask;
            dstAccessMask,  // dstAccessMask;
            dependencyFlags // dependencyFlags;
        };
    }

    VkRenderPassCreateInfo
    getRenderPassCreateInfo(const uint32_t                 attachmentsCount,
                            const uint32_t                 subpassesCount,
                            const uint32_t                 dependenciesCount,
                            const VkAttachmentDescription* attachments,
                            const VkSubpassDescription*    subpasses,
                            const VkSubpassDependency*     dependencies,
                            const VkRenderPassCreateFlags  flags,
                            const void*                    next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, // sType;
            next,                                      // pNext;
            flags,                                     // flags;
            attachmentsCount,                          // attachmentCount;
            attachments,                               // pAttachments;
            subpassesCount,                            // subpassCount;
            subpasses,                                 // pSubpasses;
            dependenciesCount,                         // dependencyCount;
            dependencies                               // pDependencies;
        };
    }

    VkRenderPass
    getRenderPassHandle(const VkRenderPassCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkRenderPass renderPass;

        ND_ASSERT_EXEC(vkCreateRenderPass(device, &createInfo, nullptr, &renderPass) == VK_SUCCESS);

        return renderPass;
    }

    RenderPass
    getRenderPass(const RenderPassConfiguration& configuration, const VkDevice device)
    {
        ND_SET_SCOPE();

        const auto subpasses = getMapped<SubpassDescription, VkSubpassDescription>(
            configuration.subpasses,
            [](const auto& subpass, const auto index)
            {
                ND_ASSERT(!subpass.resolveAttachments.size() ||
                          subpass.colorAttachments.size() == subpass.resolveAttachments.size());

                return getRenderPassSubpass(
                    subpass.pipelineBindPoint,
                    subpass.inputAttachments.size(),
                    subpass.colorAttachments.size(),
                    subpass.preserveAttachments.size(),
                    subpass.inputAttachments.data(),
                    subpass.colorAttachments.data(),
                    subpass.resolveAttachments.data(),
                    subpass.depthStencilAttachment.has_value() ? &subpass.depthStencilAttachment.value() : nullptr,
                    subpass.preserveAttachments.data(),
                    subpass.flags);
            });

        const auto createInfo = getRenderPassCreateInfo(configuration.attachments.size(),
                                                        subpasses.size(),
                                                        configuration.dependencies.size(),
                                                        configuration.attachments.data(),
                                                        subpasses.data(),
                                                        configuration.dependencies.data(),
                                                        configuration.flags,
                                                        configuration.next);

        return {getRenderPassHandle(createInfo, device)};
    }
} // namespace nd::src::graphics::vulkan
