#include "render_pass_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    RenderPass
    createRenderPass(opt<const RenderPassCfg>::ref cfg, const VkDevice device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto subpassDescriptions = getMapped<SubpassDescription, VkSubpassDescription>(
            cfg.subpassDescriptions,
            [](const auto& description, const auto index)
            {
                return VkSubpassDescription {.flags                   = description.flags,
                                             .pipelineBindPoint       = description.pipelineBindPoint,
                                             .inputAttachmentCount    = static_cast<u32>(description.inputAttachments.size()),
                                             .pInputAttachments       = description.inputAttachments.data(),
                                             .colorAttachmentCount    = static_cast<u32>(description.colorAttachments.size()),
                                             .pColorAttachments       = description.colorAttachments.data(),
                                             .pResolveAttachments     = description.resolveAttachments.data(),
                                             .pDepthStencilAttachment = description.depthStencilAttachments.data(),
                                             .preserveAttachmentCount = static_cast<u32>(description.preserveAttachments.size()),
                                             .pPreserveAttachments    = description.preserveAttachments.data()};
            });

        const auto createInfo = VkRenderPassCreateInfo {.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                                                        .pNext           = cfg.next,
                                                        .flags           = cfg.flags,
                                                        .attachmentCount = static_cast<u32>(cfg.attachmentDescriptions.size()),
                                                        .pAttachments    = cfg.attachmentDescriptions.data(),
                                                        .subpassCount    = static_cast<u32>(subpassDescriptions.size()),
                                                        .pSubpasses      = subpassDescriptions.data(),
                                                        .dependencyCount = static_cast<u32>(cfg.subpassDependencies.size()),
                                                        .pDependencies   = cfg.subpassDependencies.data()};

        VkRenderPass renderPass;

        ND_ASSERT_EXEC(vkCreateRenderPass(device, &createInfo, ND_VULKAN_ALLOCATION_CALLBACKS, &renderPass) == VK_SUCCESS);

        return renderPass;
    }
} // namespace nd::src::graphics::vulkan
