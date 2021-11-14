#include "framebuffer.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    Framebuffer::Framebuffer() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    Framebuffer::Framebuffer(const VkDevice device, const CreateInfo& createInfo)
        : device_(device)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreateFramebuffer(device_, &createInfo, nullptr, &framebuffer_) == VK_SUCCESS);
    }

    Framebuffer::Framebuffer(Framebuffer&& framebuffer) noexcept
        : device_(std::move(framebuffer.device_))
        , framebuffer_(std::move(framebuffer.framebuffer_))
    {
        ND_SET_SCOPE_LOW();

        framebuffer.framebuffer_ = VK_NULL_HANDLE;
    }

    Framebuffer&
    Framebuffer::operator=(Framebuffer&& framebuffer) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&framebuffer == this)
        {
            return *this;
        }

        device_      = framebuffer.device_;
        framebuffer_ = framebuffer.framebuffer_;

        framebuffer.framebuffer_ = VK_NULL_HANDLE;

        return *this;
    }

    Framebuffer::~Framebuffer()
    {
        ND_SET_SCOPE_LOW();

        vkDestroyFramebuffer(device_, framebuffer_, nullptr);
    }

    Framebuffer::CreateInfo
    getFramebufferCreateInfo(const VkRenderPass renderPass,
                             const uint32_t     attachmentsCount,
                             const VkImageView* attachments,
                             const uint32_t     width,
                             const uint32_t     height,
                             const uint32_t     layers) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, // sType;
            nullptr,                                   // pNext;
            0,                                         // flags;
            renderPass,                                // renderPass;
            attachmentsCount,                          // attachmentCount;
            attachments,                               // pAttachments;
            width,                                     // width;
            height,                                    // height;
            layers                                     // layers;
        };
    }

    Framebuffer
    getFramebuffer(const Framebuffer::Configuration& configuration,
                   const Framebuffer::Attachments&   attachments,
                   const VkDevice                    device,
                   const VkRenderPass                renderPass)
    {
        ND_SET_SCOPE_LOW();

        const auto createInfo = getFramebufferCreateInfo(renderPass,
                                                         attachments.size(),
                                                         attachments.data(),
                                                         configuration.width,
                                                         configuration.height,
                                                         configuration.layers);

        return Framebuffer(device, createInfo);
    }
} // namespace nd::src::graphics::vulkan
