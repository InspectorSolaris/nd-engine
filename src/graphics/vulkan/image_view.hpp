#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    class ImageView final
    {
    public:
        using CreateInfo = VkImageViewCreateInfo;

        struct Configuration final
        {
            const VkImageSubresourceRange& subresourceRange;
            const VkComponentMapping&      components;
            const VkImageViewType          viewType;
            const VkFormat                 format;
        };

        ImageView() noexcept;
        ImageView(const VkDevice device, const CreateInfo& createInfo);

        ImageView(const ImageView& imageView) = delete;
        ImageView(ImageView&& imageView) noexcept;

        ImageView&
        operator=(const ImageView& imageView) = delete;
        ImageView&
        operator=(ImageView&& imageView) noexcept;

        ~ImageView();

        constexpr VkImageView
        get() const noexcept;

    private:
        VkDevice    device_ {VK_NULL_HANDLE};
        VkImageView imageView_ {VK_NULL_HANDLE};
    };

    constexpr VkImageView
    ImageView::get() const noexcept
    {
        return imageView_;
    }

    ImageView::CreateInfo
    getImageViewCreateInfo(const VkImage                  image,
                           const VkImageViewType          viewType,
                           const VkFormat                 format,
                           const VkComponentMapping&      components,
                           const VkImageSubresourceRange& subresourceRange) noexcept;

    ImageView
    getImageView(const ImageView::Configuration& configuration, const VkDevice device, const VkImage image);
} // namespace nd::src::graphics::vulkan
