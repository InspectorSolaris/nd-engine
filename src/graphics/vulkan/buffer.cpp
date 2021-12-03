#include "buffer.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    Buffer::Buffer() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    Buffer::Buffer(const VkDevice device, const VkBufferCreateInfo& createInfo)
        : device_(device)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreateBuffer(device_, &createInfo, nullptr, &buffer_) == VK_SUCCESS);
    }

    Buffer::Buffer(Buffer&& buffer) noexcept
        : device_(std::move(buffer.device_))
        , buffer_(std::move(buffer.buffer_))
    {
        ND_SET_SCOPE_LOW();

        buffer.buffer_ = VK_NULL_HANDLE;
    }

    Buffer&
    Buffer::operator=(Buffer&& buffer) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&buffer == this)
        {
            return *this;
        }

        device_ = std::move(buffer.device_);
        buffer_ = std::move(buffer.buffer_);

        buffer.buffer_ = VK_NULL_HANDLE;

        return *this;
    }

    Buffer::~Buffer()
    {
        ND_SET_SCOPE_LOW();

        vkDestroyBuffer(device_, buffer_, nullptr);
    }

    VkBufferCreateInfo
    getBufferCreateInfo(const VkDeviceSize       size,
                        const VkBufferUsageFlags usage,
                        const VkSharingMode      sharingMode,
                        const uint32_t           queueFamilyIndicesCount,
                        const uint32_t*          queueFamilyIndices) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, // sType;
            nullptr,                              // pNext;
            0,                                    // flags;
            size,                                 // size;
            usage,                                // usage;
            sharingMode,                          // sharingMode;
            queueFamilyIndicesCount,              // queueFamilyIndexCount;
            queueFamilyIndices                    // pQueueFamilyIndices;
        };
    }
} // namespace nd::src::graphics::vulkan
