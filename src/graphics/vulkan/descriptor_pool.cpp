#include "descriptor_pool.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    DescriptorPool::DescriptorPool() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    DescriptorPool::DescriptorPool(const VkDevice device, const VkDescriptorPoolCreateInfo& createInfo)
        : device_(device)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreateDescriptorPool(device_, &createInfo, nullptr, &descriptorPool_) == VK_SUCCESS);
    }

    DescriptorPool::DescriptorPool(DescriptorPool&& descriptorPool) noexcept
        : device_(std::move(descriptorPool.device_))
        , descriptorPool_(std::move(descriptorPool.descriptorPool_))
    {
        ND_SET_SCOPE_LOW();

        descriptorPool.descriptorPool_ = VK_NULL_HANDLE;
    }

    DescriptorPool&
    DescriptorPool::operator=(DescriptorPool&& descriptorPool) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&descriptorPool == this)
        {
            return *this;
        }

        device_         = std::move(descriptorPool.device_);
        descriptorPool_ = std::move(descriptorPool.descriptorPool_);

        descriptorPool.descriptorPool_ = VK_NULL_HANDLE;

        return *this;
    }

    DescriptorPool::~DescriptorPool()
    {
        ND_SET_SCOPE_LOW();

        vkDestroyDescriptorPool(device_, descriptorPool_, nullptr);
    }

    VkDescriptorPoolCreateInfo
    getDescriptorPoolCreateInfo(const uint32_t              maxSets,
                                const uint32_t              poolSizesCount,
                                const VkDescriptorPoolSize* poolSizes) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,     // sType;
            nullptr,                                           // pNext;
            VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT, // flags;
            maxSets,                                           // maxSets;
            poolSizesCount,                                    // poolSizeCount;
            poolSizes                                          // pPoolSizes;
        };
    }

    DescriptorPool
    getDescriptorPool(const DescriptorPool::Configuration& configuration, const VkDevice device)
    {
        ND_SET_SCOPE_LOW();

        const auto createInfo = getDescriptorPoolCreateInfo(configuration.maxSets,
                                                            configuration.descriptorPoolSizes.size(),
                                                            configuration.descriptorPoolSizes.data());

        return DescriptorPool(device, createInfo);
    }
} // namespace nd::src::graphics::vulkan
