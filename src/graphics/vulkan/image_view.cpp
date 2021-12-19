#include "image_view.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VkImageViewCreateInfo
    getImageViewCreateInfo(const VkImage                  image,
                           const VkImageViewType          viewType,
                           const VkFormat                 format,
                           const VkComponentMapping&      components,
                           const VkImageSubresourceRange& subresourceRange,
                           const VkImageViewCreateFlags   flags,
                           const void*                    next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, // sType;
            next,                                     // pNext;
            flags,                                    // flags;
            image,                                    // image;
            viewType,                                 // viewType;
            format,                                   // format;
            components,                               // components;
            subresourceRange                          // subresourceRange;
        };
    }

    VkImageViewCreateInfo
    getImageViewCreateInfo(const ImageViewConfiguration& configuration)
    {
        ND_SET_SCOPE();

        return getImageViewCreateInfo(configuration.image,
                                      configuration.viewType,
                                      configuration.format,
                                      configuration.components,
                                      configuration.subresourceRange);
    }

    VkImageView
    getImageView(const VkImageViewCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkImageView imageView;

        ND_ASSERT(vkCreateImageView(device, &createInfo, nullptr, &imageView) == VK_SUCCESS);

        return imageView;
    }
} // namespace nd::src::graphics::vulkan
