#include "command_buffer.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    CommandBuffer::CommandBuffer() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    CommandBuffer::CommandBuffer(const VkDevice device, const VkCommandPool commandPool, const AllocateInfo& allocateInfo)
        : device_(device)
        , commandPool_(commandPool)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkAllocateCommandBuffers(device_, &allocateInfo, &commandBuffer_) == VK_SUCCESS);
    }

    CommandBuffer::CommandBuffer(CommandBuffer&& commandBuffer) noexcept
        : device_(std::move(commandBuffer.device_))
        , commandPool_(std::move(commandBuffer.commandPool_))
        , commandBuffer_(std::move(commandBuffer.commandBuffer_))
    {
        ND_SET_SCOPE_LOW();

        commandBuffer.commandBuffer_ = VK_NULL_HANDLE;
    }

    CommandBuffer&
    CommandBuffer::operator=(CommandBuffer&& commandBuffer) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&commandBuffer == this)
        {
            return *this;
        }

        device_        = std::move(commandBuffer.device_);
        commandPool_   = std::move(commandBuffer.commandPool_);
        commandBuffer_ = std::move(commandBuffer.commandBuffer_);

        commandBuffer.commandBuffer_ = VK_NULL_HANDLE;

        return *this;
    }

    CommandBuffer::~CommandBuffer()
    {
        ND_SET_SCOPE_LOW();

        vkFreeCommandBuffers(device_, commandPool_, 1, &commandBuffer_);
    }

    CommandBuffer::AllocateInfo
    getCommandBufferAllocateInfo(const VkCommandPool        commandPool,
                                 const VkCommandBufferLevel level,
                                 const uint32_t             commandBufferCount) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, // sType;
            nullptr,                                        // pNext;
            commandPool,                                    // commandPool;
            level,                                          // level;
            commandBufferCount                              // commandBufferCount;
        };
    }
} // namespace nd::src::graphics::vulkan
