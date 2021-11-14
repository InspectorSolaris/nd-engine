#include "buffer_view.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    BufferView::BufferView() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    BufferView::BufferView(const VkDevice device, const CreateInfo &createInfo) : device_(device)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreateBufferView(device_, &createInfo, nullptr, &bufferView_) == VK_SUCCESS);
    }

    BufferView::BufferView(BufferView &&bufferView) noexcept :
        device_(std::move(bufferView.device_)),
        bufferView_(std::move(bufferView.bufferView_))
    {
        ND_SET_SCOPE_LOW();

        bufferView.bufferView_ = VK_NULL_HANDLE;
    }

    BufferView &
    BufferView::operator=(BufferView &&bufferView) noexcept
    {
        ND_SET_SCOPE_LOW();

        if (&bufferView == this)
        {
            return *this;
        }

        device_     = std::move(bufferView.device_);
        bufferView_ = std::move(bufferView.bufferView_);

        bufferView.bufferView_ = VK_NULL_HANDLE;

        return *this;
    }

    BufferView::~BufferView()
    {
        ND_SET_SCOPE_LOW();

        vkDestroyBufferView(device_, bufferView_, nullptr);
    }

    BufferView::CreateInfo
    getBufferViewCreateInfo(const VkBuffer     buffer,
                            const VkFormat     format,
                            const VkDeviceSize offset,
                            const VkDeviceSize range) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,    // sType;
            nullptr,                                      // pNext;
            0,                                            // flags;
            buffer,                                       // buffer;
            format,                                       // format;
            offset,                                       // offset;
            range                                         // range;
        };
    }
}    // namespace nd::src::graphics::vulkan
