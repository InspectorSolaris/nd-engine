#include "descriptor_set_layout.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    DescriptorSetLayout::DescriptorSetLayout() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    DescriptorSetLayout::DescriptorSetLayout(const VkDevice device, const CreateInfo& createInfo)
        : device_(device)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreateDescriptorSetLayout(device_, &createInfo, nullptr, &descriptorSetLayout_) == VK_SUCCESS);
    }

    DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout&& descriptorSetLayout) noexcept
        : device_(std::move(descriptorSetLayout.device_))
        , descriptorSetLayout_(std::move(descriptorSetLayout.descriptorSetLayout_))
    {
        ND_SET_SCOPE_LOW();

        descriptorSetLayout.descriptorSetLayout_ = VK_NULL_HANDLE;
    }

    DescriptorSetLayout&
    DescriptorSetLayout::operator=(DescriptorSetLayout&& descriptorSetLayout) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&descriptorSetLayout == this)
        {
            return *this;
        }

        device_              = std::move(descriptorSetLayout.device_);
        descriptorSetLayout_ = std::move(descriptorSetLayout.descriptorSetLayout_);

        descriptorSetLayout.descriptorSetLayout_ = VK_NULL_HANDLE;

        return *this;
    }

    DescriptorSetLayout::~DescriptorSetLayout()
    {
        ND_SET_SCOPE_LOW();

        vkDestroyDescriptorSetLayout(device_, descriptorSetLayout_, nullptr);
    }

    DescriptorSetLayout::CreateInfo
    getDescriptorSetLayoutCreateInfo(const uint32_t bindingsCount, const VkDescriptorSetLayoutBinding* bindings) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, // sType;
            nullptr,                                             // pNext;
            0,                                                   // flags;
            bindingsCount,                                       // bindingCount;
            bindings                                             // pBindings;
        };
    }

    DescriptorSetLayout
    getDescriptorSetLayout(const DescriptorSetLayout::Configuration& configuration, const VkDevice device)
    {
        ND_SET_SCOPE_LOW();

        const auto createInfo =
            getDescriptorSetLayoutCreateInfo(configuration.bindings.size(), configuration.bindings.data());

        return DescriptorSetLayout(device, createInfo);
    }
} // namespace nd::src::graphics::vulkan
