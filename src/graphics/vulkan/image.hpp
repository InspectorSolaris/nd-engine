#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class Image final
    {
    public:
        using CreateInfo = VkImageCreateInfo;

        Image() noexcept;
        Image(const VkDevice device, const CreateInfo& createInfo);

        Image(const Image& image) = delete;
        Image(Image&& image) noexcept;

        Image&
        operator=(const Image& image) = delete;
        Image&
        operator=(Image&& image) noexcept;

        ~Image();

        constexpr VkImage
        get() const noexcept;

    private:
        VkDevice device_ {VK_NULL_HANDLE};
        VkImage  image_ {VK_NULL_HANDLE};
    };

    constexpr VkImage
    Image::get() const noexcept
    {
        return image_;
    }

    Image::CreateInfo
    getImageCreateInfo(const VkImageType           type,
                       const VkFormat              format,
                       const VkExtent3D            extent,
                       const uint32_t              mipLevels,
                       const uint32_t              arrayLevels,
                       const VkSampleCountFlagBits samples,
                       const VkImageTiling         tiling,
                       const VkImageUsageFlags     usage,
                       const VkSharingMode         sharingMode,
                       const uint32_t              queueFamilyIndicesCount,
                       const uint32_t*             queueFamilyIndices,
                       const VkImageLayout         initialLayout) noexcept;
} // namespace nd::src::graphics::vulkan
