#include "context_configuration.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    auto initializersBuilder = VulkanContextInitializersBuilder {} //
        << getInstance                                             //
        << getPhysicalDevice                                       //
        << getDevice                                               //
        << getSwapchain                                            //
        << getRenderPass                                           //
        << getSwapchainImages                                      //
        << getImageViews                                           //
        << getFramebuffers                                         //
        << getShaderModules;                                       //

    auto configurationsBuilder = VulkanContextConfigurationsBuilder {} //
        << getInstanceConfiguration                                    //
        << getPhysicalDeviceConfiguration                              //
        << getDeviceConfiguration                                      //
        << getSwapchainConfiguration                                   //
        << getRenderPassConfiguration                                  //
        << getSwapchainImageViewConfigurations                         //
        << getSwapchainFramebufferConfigurations                       //
        << getShaderModulesConfigurations;                             //

    InstanceConfiguration
    getInstanceConfiguration(const VulkanContextConfigurationExternal& configurationExternal) noexcept
    {
        ND_SET_SCOPE();

        const auto layers = getMerged(configurationExternal.layers,
                                      {
#ifndef NDEBUG
                                          "VK_LAYER_KHRONOS_validation"
#endif
                                      });

        const auto extensions = getMerged(configurationExternal.extensions, {});

        return {configurationExternal.applicationName,
                configurationExternal.engineName,
                std::move(layers),
                std::move(extensions),
                VK_MAKE_VERSION(0, 1, 0),
                VK_MAKE_VERSION(0, 1, 0),
                VK_API_VERSION_1_2};
    }

    PhysicalDeviceConfiguration
    getPhysicalDeviceConfiguration() noexcept
    {
        ND_SET_SCOPE();

        return {{},
                [](const VkPhysicalDevice            physicalDevice,
                   const VkPhysicalDeviceProperties& properties,
                   const VkPhysicalDeviceFeatures&   features)
                {
                    return 1;
                },
                {"VK_KHR_swapchain"},
                VK_QUEUE_GRAPHICS_BIT};
    }

    DeviceConfiguration
    getDeviceConfiguration(const PhysicalDeviceConfiguration& physicalDeviceConfiguration) noexcept
    {
        ND_SET_SCOPE();

        return {physicalDeviceConfiguration.features, physicalDeviceConfiguration.extensions};
    }

    SwapchainConfiguration
    getSwapchainConfiguration(const VkPhysicalDevice physicalDevice,
                              const VkSurfaceKHR     surface,
                              const uint32_t         width,
                              const uint32_t         height) noexcept
    {
        ND_SET_SCOPE();

        return {physicalDevice,
                surface,
                {width, height},
                1,
                1,
                true,
                VK_FORMAT_B8G8R8A8_SRGB,
                VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                VK_PRESENT_MODE_IMMEDIATE_KHR,
                VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR};
    }

    RenderPassConfiguration
    getRenderPassConfiguration(const SwapchainConfiguration& swapchainConfiguration) noexcept
    {
        ND_SET_SCOPE();

        using Attachments  = std::vector<AttachmentDescription>;
        using Subpasses    = std::vector<SubpassDescription>;
        using Dependencies = std::vector<SubpassDependency>;

        const auto attachments = Attachments {getRenderPassAttachment(swapchainConfiguration.imageFormat,
                                                                      VK_SAMPLE_COUNT_1_BIT,
                                                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                                      VK_ATTACHMENT_STORE_OP_STORE,
                                                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                                                      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)};

        const auto subpasses =
            Subpasses {{{}, {{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}}, {}, {}, {}, VK_PIPELINE_BIND_POINT_GRAPHICS}};

        const auto dependencies = Dependencies {{VK_SUBPASS_EXTERNAL,
                                                 0,
                                                 VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                                 VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                                 0,
                                                 VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                                                 {}}};

        return {attachments, subpasses, dependencies};
    }

    ImageViewConfiguration
    getSwapchainImageViewConfiguration(const SwapchainConfiguration& swapchainConfiguration, const VkImage image) noexcept
    {
        ND_SET_SCOPE();

        return {{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
                {VK_COMPONENT_SWIZZLE_IDENTITY,
                 VK_COMPONENT_SWIZZLE_IDENTITY,
                 VK_COMPONENT_SWIZZLE_IDENTITY,
                 VK_COMPONENT_SWIZZLE_IDENTITY},
                VK_IMAGE_VIEW_TYPE_2D,
                swapchainConfiguration.imageFormat,
                image};
    }

    FramebufferConfiguration
    getSwapchainFramebufferConfiguration(const SwapchainConfiguration& swapchainConfiguration,
                                         const VkImageView             imageView,
                                         const VkRenderPass            renderPass) noexcept
    {
        ND_SET_SCOPE();

        return {{imageView},
                renderPass,
                swapchainConfiguration.imageExtent.width,
                swapchainConfiguration.imageExtent.height,
                swapchainConfiguration.imageArrayLayers};
    }

    std::vector<ImageViewConfiguration>
    getSwapchainImageViewConfigurations(const SwapchainConfiguration& swapchainConfiguration,
                                        const std::vector<Image>&     images) noexcept
    {
        ND_SET_SCOPE();

        return getMapped<Image, ImageViewConfiguration>(images,
                                                        [&swapchainConfiguration](const auto image, const auto index)
                                                        {
                                                            return getSwapchainImageViewConfiguration(swapchainConfiguration,
                                                                                                      image.handle);
                                                        });
    }

    std::vector<FramebufferConfiguration>
    getSwapchainFramebufferConfigurations(const SwapchainConfiguration& swapchainConfiguration,
                                          const std::vector<ImageView>  imageViews,
                                          const VkRenderPass            renderPass) noexcept
    {
        ND_SET_SCOPE();

        return getMapped<ImageView, FramebufferConfiguration>(
            imageViews,
            [&swapchainConfiguration, renderPass](const auto imageView, const auto index)
            {
                return getSwapchainFramebufferConfiguration(swapchainConfiguration, imageView.handle, renderPass);
            });
    }

    std::vector<ShaderModuleConfiguration>
    getShaderModulesConfigurations() noexcept
    {
        ND_SET_SCOPE();

        return {{"src/graphics/vulkan/shaders/vert.spv", VK_SHADER_STAGE_VERTEX_BIT},
                {"src/graphics/vulkan/shaders/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT}};
    }
} // namespace nd::src::graphics::vulkan
