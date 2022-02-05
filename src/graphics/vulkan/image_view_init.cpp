#include "image_view_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    ImageView
    createImageView(opt<const ImageViewCfg>::ref cfg, const VkDevice device, const VkImage image) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto createInfo = VkImageViewCreateInfo {.sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                                       .pNext            = cfg.next,
                                                       .flags            = cfg.flags,
                                                       .image            = image,
                                                       .viewType         = cfg.type,
                                                       .format           = cfg.format,
                                                       .components       = cfg.components,
                                                       .subresourceRange = cfg.subresourceRange};

        VkImageView imageView;

        ND_ASSERT_EXEC(vkCreateImageView(device, &createInfo, ND_VULKAN_ALLOCATION_CALLBACKS, &imageView) == VK_SUCCESS);

        return imageView;
    }
} // namespace nd::src::graphics::vulkan
