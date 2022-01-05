#include "buffer.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VkBufferCreateInfo
    getBufferCreateInfo(const VkDeviceSize        size,
                        const VkBufferUsageFlags  usage,
                        const VkSharingMode       sharingMode,
                        const uint32_t            queueFamilyIndicesCount,
                        const uint32_t*           queueFamilyIndices,
                        const VkBufferCreateFlags flags,
                        const void*               next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, // sType;
            next,                                 // pNext;
            flags,                                // flags;
            size,                                 // size;
            usage,                                // usage;
            sharingMode,                          // sharingMode;
            queueFamilyIndicesCount,              // queueFamilyIndexCount;
            queueFamilyIndices                    // pQueueFamilyIndices;
        };
    }

    VkBuffer
    getBufferHandle(const VkBufferCreateInfo& createInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkBuffer buffer;

        ND_ASSERT_EXEC(vkCreateBuffer(device, &createInfo, nullptr, &buffer) == VK_SUCCESS);

        return buffer;
    }

    Buffer
    getBuffer(const BufferConfiguration& configuration, const VkDevice device)
    {
        ND_SET_SCOPE();

        const auto queueFamiliesIndices = getQueueFamiliesIndices(configuration.queueFamilies);

        const auto createInfo = getBufferCreateInfo(configuration.size,
                                                    configuration.usage,
                                                    queueFamiliesIndices.size() > 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
                                                    queueFamiliesIndices.size(),
                                                    queueFamiliesIndices.data());

        return getBufferHandle(createInfo, device);
    }

    std::vector<Buffer>
    getBuffers(const std::vector<BufferConfiguration>& configurations, const VkDevice device)
    {
        ND_SET_SCOPE();

        return getMapped<BufferConfiguration, Buffer>(configurations,
                                                      [device](const auto& configuration, const auto index)
                                                      {
                                                          return getBuffer(configuration, device);
                                                      });
    }
} // namespace nd::src::graphics::vulkan
