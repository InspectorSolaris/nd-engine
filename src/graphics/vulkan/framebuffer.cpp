#include "framebuffer.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

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

    VkFramebuffer
    getFramebufferHandle(const VkFramebufferCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkFramebuffer framebuffer;

        ND_ASSERT_EXEC(vkCreateFramebuffer(device, &createInfo, nullptr, &framebuffer) == VK_SUCCESS);

        return framebuffer;
    }

    Framebuffer
    getFramebuffer(const FramebufferConfiguration& configuration, const VkDevice device)
    {
        ND_SET_SCOPE();

        const auto createInfo = getFramebufferCreateInfo(configuration.renderPass,
                                                         configuration.attachments.size(),
                                                         configuration.attachments.data(),
                                                         configuration.width,
                                                         configuration.height,
                                                         configuration.layers,
                                                         configuration.flags,
                                                         configuration.next);

        return {getFramebufferHandle(createInfo, device)};
    }

    std::vector<Framebuffer>
    getFramebuffers(const std::vector<FramebufferConfiguration>& configurations, const VkDevice device)
    {
        ND_SET_SCOPE();

        return getMapped<FramebufferConfiguration, Framebuffer>(configurations,
                                                                [device](const auto& configuration, const auto index)
                                                                {
                                                                    return getFramebuffer(configuration, device);
                                                                });
    }
} // namespace nd::src::graphics::vulkan
