#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class Buffer final
    {
    public:
        using CreateInfo = VkBufferCreateInfo;

        Buffer() noexcept;
        Buffer(const VkDevice device, const CreateInfo& createInfo);

        Buffer(const Buffer& buffer) = delete;
        Buffer(Buffer&& buffer) noexcept;

        Buffer&
        operator=(const Buffer& buffer) = delete;
        Buffer&
        operator=(Buffer&& buffer) noexcept;

        ~Buffer();

        constexpr VkBuffer
        get() const noexcept;

    private:
        VkDevice device_ {VK_NULL_HANDLE};
        VkBuffer buffer_ {VK_NULL_HANDLE};
    };

    constexpr VkBuffer
    Buffer::get() const noexcept
    {
        return buffer_;
    }

    Buffer::CreateInfo
    getBufferCreateInfo(const VkDeviceSize       size,
                        const VkBufferUsageFlags usage,
                        const VkSharingMode      sharingMode,
                        const uint32_t           queueFamilyIndicesCount,
                        const uint32_t*          queueFamilyIndices) noexcept;
} // namespace nd::src::graphics::vulkan
