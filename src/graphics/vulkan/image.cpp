#include "image.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
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
                       const VkImageLayout         initialLayout,
                       const VkImageCreateFlags    flags,
                       const void*                 next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, // sType;
            next,                                // pNext;
            flags,                               // flags;
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

    VkImage
    getImageHandle(const VkImageCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkImage image;

        ND_ASSERT_EXEC(vkCreateImage(device, &createInfo, nullptr, &image) == VK_SUCCESS);

        return image;
    }
} // namespace nd::src::graphics::vulkan
