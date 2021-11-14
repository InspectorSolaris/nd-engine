#include "image_view.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    ImageView::ImageView() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    ImageView::ImageView(const VkDevice device, const CreateInfo& createInfo)
        : device_(device)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreateImageView(device_, &createInfo, nullptr, &imageView_) == VK_SUCCESS);
    }

    ImageView::ImageView(ImageView&& imageView) noexcept
        : device_(std::move(imageView.device_))
        , imageView_(std::move(imageView.imageView_))
    {
        ND_SET_SCOPE_LOW();

        imageView.imageView_ = VK_NULL_HANDLE;
    }

    ImageView&
    ImageView::operator=(ImageView&& imageView) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&imageView == this)
        {
            return *this;
        }

        device_    = std::move(imageView.device_);
        imageView_ = std::move(imageView.imageView_);

        imageView.imageView_ = VK_NULL_HANDLE;

        return *this;
    }

    ImageView::~ImageView()
    {
        ND_SET_SCOPE_LOW();

        vkDestroyImageView(device_, imageView_, nullptr);
    }

    ImageView::CreateInfo
    getImageViewCreateInfo(const VkImage                  image,
                           const VkImageViewType          viewType,
                           const VkFormat                 format,
                           const VkComponentMapping&      components,
                           const VkImageSubresourceRange& subresourceRange) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, // sType;
            nullptr,                                  // pNext;
            0,                                        // flags;
            image,                                    // image;
            viewType,                                 // viewType;
            format,                                   // format;
            components,                               // components;
            subresourceRange                          // subresourceRange;
        };
    }

    ImageView
    getImageView(const ImageView::Configuration& configuration, const VkDevice device, const VkImage image)
    {
        ND_SET_SCOPE_LOW();

        const auto createInfo = getImageViewCreateInfo(image,
                                                       configuration.viewType,
                                                       configuration.format,
                                                       configuration.components,
                                                       configuration.subresourceRange);

        return ImageView(device, createInfo);
    }
} // namespace nd::src::graphics::vulkan
