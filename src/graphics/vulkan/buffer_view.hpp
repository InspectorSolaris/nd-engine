#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class BufferView final
    {
    public:
        using CreateInfo = VkBufferViewCreateInfo;

        BufferView() noexcept;
        BufferView(const VkDevice device, const CreateInfo& createInfo);

        BufferView(const BufferView& bufferView) = delete;
        BufferView(BufferView&& bufferView) noexcept;

        BufferView&
        operator=(const BufferView& bufferView) = delete;
        BufferView&
        operator=(BufferView&& bufferView) noexcept;

        ~BufferView();

        constexpr VkBufferView
        get() const noexcept;

    private:
        VkDevice     device_ {VK_NULL_HANDLE};
        VkBufferView bufferView_ {VK_NULL_HANDLE};
    };

    constexpr VkBufferView
    BufferView::get() const noexcept
    {
        return bufferView_;
    }

    BufferView::CreateInfo
    getBufferViewCreateInfo(const VkBuffer     buffer,
                            const VkFormat     format,
                            const VkDeviceSize offset,
                            const VkDeviceSize range) noexcept;
} // namespace nd::src::graphics::vulkan
