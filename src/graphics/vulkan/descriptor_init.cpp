#include "descriptor_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    DescriptorPool
    createDescriptorPool(opt<const DescriptorPoolCfg>::ref cfg, const VkDevice device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto createInfo = VkDescriptorPoolCreateInfo {.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
                                                            .pNext         = cfg.next,
                                                            .flags         = cfg.flags,
                                                            .maxSets       = cfg.maxSets,
                                                            .poolSizeCount = static_cast<u32>(cfg.sizes.size()),
                                                            .pPoolSizes    = cfg.sizes.data()};

        VkDescriptorPool descriptorPool;

        ND_ASSERT_EXEC(vkCreateDescriptorPool(device, &createInfo, ND_VULKAN_ALLOCATION_CALLBACKS, &descriptorPool) == VK_SUCCESS);

        return descriptorPool;
    }

    DescriptorSetLayout
    createDescriptorSetLayout(opt<const DescriptorSetLayoutCfg>::ref cfg, const VkDevice device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto createInfo = VkDescriptorSetLayoutCreateInfo {.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                                                                 .pNext        = cfg.next,
                                                                 .flags        = cfg.flags,
                                                                 .bindingCount = static_cast<u32>(cfg.bindings.size()),
                                                                 .pBindings    = cfg.bindings.data()};

        VkDescriptorSetLayout descriptorSetLayout;

        ND_ASSERT_EXEC(vkCreateDescriptorSetLayout(device, &createInfo, ND_VULKAN_ALLOCATION_CALLBACKS, &descriptorSetLayout) == VK_SUCCESS);

        return descriptorSetLayout;
    }

    DescriptorSetLayoutObjects
    createDescriptorSetLayoutObjects(opt<const DescriptorSetLayoutObjectsCfg>::ref cfg, const VkDevice device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.mesh = createDescriptorSetLayout(cfg.mesh, device)};
    }
} // namespace nd::src::graphics::vulkan
