#include "framebuffer_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    Framebuffer
    createFramebuffer(opt<const FramebufferCfg>::ref cfg, const VkDevice device, const vec<VkImageView>& imageViews) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto createInfo = VkFramebufferCreateInfo {.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                                                         .pNext           = cfg.next,
                                                         .flags           = cfg.flags,
                                                         .renderPass      = cfg.renderPass,
                                                         .attachmentCount = static_cast<u32>(imageViews.size()),
                                                         .pAttachments    = imageViews.data(),
                                                         .width           = static_cast<u32>(cfg.width),
                                                         .height          = static_cast<u32>(cfg.height),
                                                         .layers          = static_cast<u32>(cfg.layers)};

        VkFramebuffer framebuffer;

        ND_ASSERT_EXEC(vkCreateFramebuffer(device, &createInfo, ND_VULKAN_ALLOCATION_CALLBACKS, &framebuffer) == VK_SUCCESS);

        return framebuffer;
    }
} // namespace nd::src::graphics::vulkan
