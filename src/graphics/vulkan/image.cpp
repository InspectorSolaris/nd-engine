#include "image.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

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

    std::vector<Image>
    getSwapchainImages(const VkSwapchainKHR swapchain, const VkDevice device) noexcept
    {
        ND_SET_SCOPE();

        uint32_t count;

        vkGetSwapchainImagesKHR(device, swapchain, &count, nullptr);

        auto images = std::vector<VkImage>(count);

        vkGetSwapchainImagesKHR(device, swapchain, &count, images.data());

        return images;
    }
} // namespace nd::src::graphics::vulkan
