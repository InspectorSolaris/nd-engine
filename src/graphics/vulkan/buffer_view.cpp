#include "buffer_view.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    VkBufferViewCreateInfo
    getBufferViewCreateInfo(const VkBuffer                buffer,
                            const VkFormat                format,
                            const VkDeviceSize            offset,
                            const VkDeviceSize            range,
                            const VkBufferViewCreateFlags flags,
                            const void*                   next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO, // sType;
            next,                                      // pNext;
            flags,                                     // flags;
            buffer,                                    // buffer;
            format,                                    // format;
            offset,                                    // offset;
            range                                      // range;
        };
    }

    VkBufferView
    getBufferViewHandle(const VkBufferViewCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkBufferView bufferView;

        ND_ASSERT(vkCreateBufferView(device, &createInfo, nullptr, &bufferView) == VK_SUCCESS);

        return bufferView;
    }
} // namespace nd::src::graphics::vulkan
