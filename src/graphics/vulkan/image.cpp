#include "image.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    Image::Image() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    Image::Image(const VkDevice device, const VkImageCreateInfo& createInfo)
        : device_(device)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreateImage(device_, &createInfo, nullptr, &image_) == VK_SUCCESS);
    }

    Image::Image(Image&& image) noexcept
        : device_(std::move(image.device_))
        , image_(std::move(image.image_))
    {
        ND_SET_SCOPE_LOW();

        image.image_ = VK_NULL_HANDLE;
    }

    Image&
    Image::operator=(Image&& image) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&image == this)
        {
            return *this;
        }

        device_ = std::move(image.device_);
        image_  = std::move(image.image_);

        image.image_ = VK_NULL_HANDLE;

        return *this;
    }

    Image::~Image()
    {
        ND_SET_SCOPE_LOW();

        vkDestroyImage(device_, image_, nullptr);
    }

    VkImageCreateInfo
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
                       const VkImageLayout         initialLayout) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, // sType;
            nullptr,                             // pNext;
            0,                                   // flags;
            type,                                // imageType;
            format,                              // format;
            extent,                              // extent;
            mipLevels,                           // mipLevels;
            arrayLevels,                         // arrayLayers;
            samples,                             // samples;
            tiling,                              // tiling;
            usage,                               // usage;
            sharingMode,                         // sharingMode;
            queueFamilyIndicesCount,             // queueFamilyIndexCount;
            queueFamilyIndices,                  // pQueueFamilyIndices;
            initialLayout                        // initialLayout;
        };
    }
} // namespace nd::src::graphics::vulkan
