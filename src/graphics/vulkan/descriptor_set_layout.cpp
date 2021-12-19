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

    VkDescriptorSetLayoutCreateInfo
    getDescriptorSetLayoutCreateInfo(const DescriptorSetLayoutConfiguration& configuration)
    {
        ND_SET_SCOPE();

        return getDescriptorSetLayoutCreateInfo(configuration.bindings.size(), configuration.bindings.data());
    }

    VkDescriptorSetLayout
    getDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkDescriptorSetLayout descriptorSetLayout;

        ND_ASSERT(vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &descriptorSetLayout) == VK_SUCCESS);

        return descriptorSetLayout;
    }
} // namespace nd::src::graphics::vulkan
