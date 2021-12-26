#include "descriptor_set_layout.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VkDescriptorSetLayoutCreateInfo
    getDescriptorSetLayoutCreateInfo(const uint32_t                         bindingsCount,
                                     const VkDescriptorSetLayoutBinding*    bindings,
                                     const VkDescriptorSetLayoutCreateFlags flags,
                                     const void*                            next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, // sType;
            next,                                                // pNext;
            flags,                                               // flags;
            bindingsCount,                                       // bindingCount;
            bindings                                             // pBindings;
        };
    }

    VkDescriptorSetLayout
    getDescriptorSetLayoutHandle(const VkDescriptorSetLayoutCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkDescriptorSetLayout descriptorSetLayout;

        ND_ASSERT_EXEC(vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &descriptorSetLayout) == VK_SUCCESS);

        return descriptorSetLayout;
    }

    VkDescriptorSetLayout
    getDescriptorSetLayout(const DescriptorSetLayoutConfiguration& configuration, const VkDevice device)
    {
        ND_SET_SCOPE();

        const auto createInfo = getDescriptorSetLayoutCreateInfo(configuration.bindings.size(),
                                                                 configuration.bindings.data(),
                                                                 configuration.flags,
                                                                 configuration.next);

        return getDescriptorSetLayoutHandle(createInfo, device);
    }
} // namespace nd::src::graphics::vulkan
