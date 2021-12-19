#include "framebuffer.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VkFramebufferCreateInfo
    getFramebufferCreateInfo(const VkRenderPass             renderPass,
                             const uint32_t                 attachmentsCount,
                             const VkImageView*             attachments,
                             const uint32_t                 width,
                             const uint32_t                 height,
                             const uint32_t                 layers,
                             const VkFramebufferCreateFlags flags,
                             const void*                    next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, // sType;
            next,                                      // pNext;
            flags,                                     // flags;
            renderPass,                                // renderPass;
            attachmentsCount,                          // attachmentCount;
            attachments,                               // pAttachments;
            width,                                     // width;
            height,                                    // height;
            layers                                     // layers;
        };
    }

    VkFramebufferCreateInfo
    getFramebufferCreateInfo(const FramebufferConfiguration& configuration)
    {
        ND_SET_SCOPE();

        return getFramebufferCreateInfo(configuration.renderPass,
                                        configuration.attachments.size(),
                                        configuration.attachments.data(),
                                        configuration.width,
                                        configuration.height,
                                        configuration.layers);
    }

    VkFramebuffer
    getFramebuffer(const VkFramebufferCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkFramebuffer framebuffer;

        ND_ASSERT(vkCreateFramebuffer(device, &createInfo, nullptr, &framebuffer) == VK_SUCCESS);

        return framebuffer;
    }
} // namespace nd::src::graphics::vulkan
