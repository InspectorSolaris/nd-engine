#include "swapchain.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    Swapchain::Swapchain() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    Swapchain::Swapchain(const VkDevice device, const QueueFamilies& queueFamilies, const CreateInfo& createInfo)
        : device_(device)
        , queueFamilies_(queueFamilies)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreateSwapchainKHR(device_, &createInfo, nullptr, &swapchain_) == VK_SUCCESS);
    }

    Swapchain::Swapchain(Swapchain&& swapchain) noexcept
        : device_(std::move(swapchain.device_))
        , swapchain_(std::move(swapchain.swapchain_))
        , queueFamilies_(std::move(swapchain.queueFamilies_))
    {
        ND_SET_SCOPE_LOW();

        swapchain.swapchain_     = VK_NULL_HANDLE;
        swapchain.queueFamilies_ = {};
    }

    Swapchain&
    Swapchain::operator=(Swapchain&& swapchain) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&swapchain == this)
        {
            return *this;
        }

        device_        = std::move(swapchain.device_);
        swapchain_     = std::move(swapchain.swapchain_);
        queueFamilies_ = std::move(swapchain.queueFamilies_);

        swapchain.swapchain_     = VK_NULL_HANDLE;
        swapchain.queueFamilies_ = {};

        return *this;
    }

    Swapchain::~Swapchain()
    {
        ND_SET_SCOPE_LOW();

        vkDestroySwapchainKHR(device_, swapchain_, nullptr);
    }

    bool
    isFormatSupported(const Swapchain::Configuration& configuration, const Surface::Formats& formats) noexcept
    {
        ND_SET_SCOPE_LOW();

        return std::any_of(formats.begin(),
                           formats.end(),
                           [&configuration](const auto& format)
                           {
                               return configuration.imageFormat == format.format &&
                                   configuration.imageColorSpace == format.colorSpace;
                           });
    }

    bool
    isPresentModeSupported(const Swapchain::Configuration& configuration, const Surface::PresentModes& presentModes) noexcept
    {
        ND_SET_SCOPE_LOW();

        return std::any_of(presentModes.begin(),
                           presentModes.end(),
                           [&configuration](const auto& presentMode)
                           {
                               return configuration.presentMode == presentMode;
                           });
    }

    bool
    isImageUsageSupported(const Swapchain::Configuration& configuration, const Surface::Capabilities& capabilities) noexcept
    {
        ND_SET_SCOPE_LOW();

        return isSubmask(capabilities.supportedUsageFlags, configuration.imageUsage);
    }

    bool
    isTransformSupported(const Swapchain::Configuration& configuration, const Surface::Capabilities& capabilities) noexcept
    {
        ND_SET_SCOPE_LOW();

        return isSubmask(capabilities.supportedTransforms, configuration.transform);
    }

    bool
    isCompositeAlphaSupported(const Swapchain::Configuration& configuration,
                              const Surface::Capabilities&    capabilities) noexcept
    {
        ND_SET_SCOPE_LOW();

        return isSubmask(capabilities.supportedCompositeAlpha, configuration.compositeAlpha);
    }

    uint32_t
    getMinImagesCount(const Swapchain::Configuration& configuration, const Surface::Capabilities& capabilities) noexcept
    {
        ND_SET_SCOPE_LOW();

        return capabilities.maxImageCount
            ? std::clamp(configuration.minImagesCount, capabilities.minImageCount, capabilities.maxImageCount)
            : std::max(capabilities.minImageCount, configuration.minImagesCount);
    }

    VkExtent2D
    getImageExtent(const Swapchain::Configuration& configuration, const Surface::Capabilities& capabilities) noexcept
    {
        ND_SET_SCOPE_LOW();

        return capabilities.currentExtent.width != 0xFFFFFFFF || capabilities.currentExtent.height != 0xFFFFFFFF
            ? capabilities.currentExtent
            : VkExtent2D {std::clamp(configuration.imageExtent.width,
                                     capabilities.minImageExtent.width,
                                     capabilities.maxImageExtent.width),
                          std::clamp(configuration.imageExtent.height,
                                     capabilities.minImageExtent.height,
                                     capabilities.maxImageExtent.height)};
    }

    uint32_t
    getImageArrayLayers(const Swapchain::Configuration& configuration, const Surface::Capabilities& capabilities) noexcept
    {
        ND_SET_SCOPE_LOW();

        return std::clamp(configuration.imageArrayLayers, 1U, capabilities.maxImageArrayLayers);
    }

    Swapchain::QueueFamilies
    getSwapchainQueueFamilies(const Device::QueueFamilies& deviceQueueFamilies,
                              const VkPhysicalDevice       physicalDevice,
                              const VkSurfaceKHR           surface) noexcept
    {
        ND_SET_SCOPE_LOW();

        Swapchain::QueueFamilies swapchainQueueFamilies;

        for(const auto queueFamily: deviceQueueFamilies)
        {
            VkBool32 isSupported;

            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamily.index, surface, &isSupported);

            if(isSupported)
            {
                swapchainQueueFamilies.emplace_back(&queueFamily);
            }
        }

        return swapchainQueueFamilies;
    }

    Swapchain::QueueFamiliesIndicies
    getSwapchainQueueFamiliesIndices(const Swapchain::QueueFamilies& swapchainQueueFamilies) noexcept
    {
        ND_SET_SCOPE_LOW();

        auto swapchainQueueFamiliesIndices = std::vector<uint32_t>(swapchainQueueFamilies.size());

        std::transform(swapchainQueueFamilies.begin(),
                       swapchainQueueFamilies.end(),
                       swapchainQueueFamiliesIndices.begin(),
                       [](const auto& swapchainQueueFamily)
                       {
                           return swapchainQueueFamily->index;
                       });

        return swapchainQueueFamiliesIndices;
    }

    Swapchain::CreateInfo
    getSwapchainCreateInfo(const VkSurfaceKHR                  surface,
                           const uint32_t                      minImagesCount,
                           const VkFormat                      imageFormat,
                           const VkColorSpaceKHR               imageColorSpace,
                           const VkExtent2D                    imageExtent,
                           const uint32_t                      imageArrayLayers,
                           const VkImageUsageFlags             imageUsage,
                           const VkSharingMode                 imageSharingMode,
                           const uint32_t                      queueFamilyIndicesCount,
                           const uint32_t*                     queueFamilyIndices,
                           const VkSurfaceTransformFlagBitsKHR preTransform,
                           const VkCompositeAlphaFlagBitsKHR   compositeAlpha,
                           const VkPresentModeKHR              presentMode,
                           const VkBool32                      clipped,
                           const VkSwapchainKHR                oldSwapchain) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR, // sType;
            nullptr,                                     // pNext;
            0,                                           // flags;
            surface,                                     // surface;
            minImagesCount,                              // minImageCount;
            imageFormat,                                 // imageFormat;
            imageColorSpace,                             // imageColorSpace;
            imageExtent,                                 // imageExtent;
            imageArrayLayers,                            // imageArrayLayers;
            imageUsage,                                  // imageUsage;
            imageSharingMode,                            // imageSharingMode;
            queueFamilyIndicesCount,                     // queueFamilyIndexCount;
            queueFamilyIndices,                          // pQueueFamilyIndices;
            preTransform,                                // preTransform;
            compositeAlpha,                              // compositeAlpha;
            presentMode,                                 // presentMode;
            clipped,                                     // clipped;
            oldSwapchain,                                // oldSwapchain;
        };
    }

    Swapchain
    getSwapchain(const Swapchain::Configuration& configuration,
                 const Device::QueueFamilies&    deviceQueueFamilies,
                 const VkPhysicalDevice          physicalDevice,
                 const VkDevice                  device,
                 const VkSurfaceKHR              surface)
    {
        ND_SET_SCOPE_LOW();

        const auto formats      = getSurfaceFormats(physicalDevice, surface);
        const auto presentModes = getSurfacePresentModes(physicalDevice, surface);
        const auto capabilities = getSurfaceCapabilities(physicalDevice, surface);

        const auto swapchainQueueFamilies        = getSwapchainQueueFamilies(deviceQueueFamilies, physicalDevice, surface);
        const auto swapchainQueueFamiliesIndices = getSwapchainQueueFamiliesIndices(swapchainQueueFamilies);

        ND_ASSERT(swapchainQueueFamilies.size());
        ND_ASSERT(isFormatSupported(configuration, formats));
        ND_ASSERT(isPresentModeSupported(configuration, presentModes));
        ND_ASSERT(isImageUsageSupported(configuration, capabilities));
        ND_ASSERT(isTransformSupported(configuration, capabilities));
        ND_ASSERT(isCompositeAlphaSupported(configuration, capabilities));

        const auto minImagesCount   = getMinImagesCount(configuration, capabilities);
        const auto imageExtent      = getImageExtent(configuration, capabilities);
        const auto imageArrayLayers = getImageArrayLayers(configuration, capabilities);

        const auto createInfo = getSwapchainCreateInfo(surface,
                                                       minImagesCount,
                                                       configuration.imageFormat,
                                                       configuration.imageColorSpace,
                                                       imageExtent,
                                                       imageArrayLayers,
                                                       configuration.imageUsage,
                                                       swapchainQueueFamilies.size() > 1 ? VK_SHARING_MODE_CONCURRENT
                                                                                         : VK_SHARING_MODE_EXCLUSIVE,
                                                       swapchainQueueFamiliesIndices.size(),
                                                       swapchainQueueFamiliesIndices.data(),
                                                       configuration.transform,
                                                       configuration.compositeAlpha,
                                                       configuration.presentMode,
                                                       configuration.clipped,
                                                       VK_NULL_HANDLE);

        return Swapchain(device, swapchainQueueFamilies, createInfo);
    }

    Swapchain::Images
    getSwapchainImages(const VkDevice device, const VkSwapchainKHR swapchain) noexcept
    {
        ND_SET_SCOPE_LOW();

        uint32_t count;

        vkGetSwapchainImagesKHR(device, swapchain, &count, nullptr);

        auto images = std::vector<VkImage>(count);

        vkGetSwapchainImagesKHR(device, swapchain, &count, images.data());

        return images;
    }

    Swapchain::ImageViews
    getSwapchainImageViews(const VkDevice                  device,
                           const Swapchain::Images&        images,
                           const Swapchain::Configuration& configuration) noexcept
    {
        ND_SET_SCOPE_LOW();

        auto imageViews = Swapchain::ImageViews {};

        imageViews.reserve(images.size());

        for(const auto& image: images)
        {
            imageViews.push_back(getImageView(
                {
                    {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
                    {   },
                    VK_IMAGE_VIEW_TYPE_2D,
                    configuration.imageFormat
            },
                device,
                image));
        }

        return imageViews;
    }

    Swapchain::Framebuffers
    getSwapchainFramebuffers(const VkDevice                  device,
                             const VkRenderPass              renderPass,
                             const Swapchain::ImageViews&    imageViews,
                             const Swapchain::Configuration& configuration) noexcept
    {
        ND_SET_SCOPE_LOW();

        auto framebuffers = Swapchain::Framebuffers {};

        framebuffers.reserve(imageViews.size());

        for(const auto& imageView: imageViews)
        {
            const auto attachments = Framebuffer::Attachments {1, imageView.get()};

            framebuffers.push_back(getFramebuffer(
                {configuration.imageExtent.width, configuration.imageExtent.height, configuration.imageArrayLayers},
                attachments,
                device,
                renderPass));
        }

        return framebuffers;
    }
} // namespace nd::src::graphics::vulkan
