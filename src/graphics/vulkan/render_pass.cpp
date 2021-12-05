#include "render_pass.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    RenderPass::RenderPass() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    RenderPass::RenderPass(const VkDevice device, const VkRenderPassCreateInfo& createInfo)
        : device_(device)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreateRenderPass(device_, &createInfo, nullptr, &renderPass_) == VK_SUCCESS);
    }

    RenderPass::RenderPass(RenderPass&& renderPass) noexcept
        : device_(std::move(renderPass.device_))
        , renderPass_(std::move(renderPass.renderPass_))
    {
        ND_SET_SCOPE_LOW();

        renderPass.renderPass_ = VK_NULL_HANDLE;
    }

    RenderPass&
    RenderPass::operator=(RenderPass&& renderPass) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&renderPass == this)
        {
            return *this;
        }

        device_     = std::move(renderPass.device_);
        renderPass_ = std::move(renderPass.renderPass_);

        renderPass.renderPass_ = VK_NULL_HANDLE;

        return *this;
    }

    RenderPass::~RenderPass()
    {
        ND_SET_SCOPE_LOW();

        vkDestroyRenderPass(device_, renderPass_, nullptr);
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
        ND_SET_SCOPE_LOW();

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
                         const VkAttachmentReference*    depthStencilAttachments,
                         const uint32_t*                 preserveAttachments,
                         const VkSubpassDescriptionFlags flags) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            flags,                    // flags;
            pipelineBindPoint,        // pipelineBindPoint;
            inputAttachmentsCount,    // inputAttachmentCount;
            inputAttachments,         // pInputAttachments;
            colorAttachmentsCount,    // colorAttachmentCount;
            colorAttachments,         // pColorAttachments;
            resolveAttachments,       // pResolveAttachments;
            depthStencilAttachments,  // pDepthStencilAttachment;
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
        ND_SET_SCOPE_LOW();

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
        ND_SET_SCOPE_LOW();

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

    RenderPass
    getRenderPass(const RenderPass::Configuration& configuration, const VkDevice device)
    {
        ND_SET_SCOPE_LOW();

        const auto createInfo = getRenderPassCreateInfo(configuration.attachments.size(),
                                                        configuration.subpasses.size(),
                                                        configuration.dependencies.size(),
                                                        configuration.attachments.data(),
                                                        configuration.subpasses.data(),
                                                        configuration.dependencies.data());

        return RenderPass(device, createInfo);
    }
} // namespace nd::src::graphics::vulkan
