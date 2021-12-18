#include "command_buffer.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    CommandBuffer::CommandBuffer() noexcept
    {
        ND_SET_SCOPE();
    }

    CommandBuffer::CommandBuffer(const VkDevice                     device,
                                 const VkCommandPool                commandPool,
                                 const VkCommandBufferAllocateInfo& allocateInfo)
        : device_(device)
        , commandPool_(commandPool)
    {
        ND_SET_SCOPE();

        ND_ASSERT(vkAllocateCommandBuffers(device_, &allocateInfo, &commandBuffer_) == VK_SUCCESS);
    }

    CommandBuffer::CommandBuffer(CommandBuffer&& commandBuffer) noexcept
        : device_(std::move(commandBuffer.device_))
        , commandPool_(std::move(commandBuffer.commandPool_))
        , commandBuffer_(std::move(commandBuffer.commandBuffer_))
    {
        ND_SET_SCOPE();

        commandBuffer.commandBuffer_ = VK_NULL_HANDLE;
    }

    CommandBuffer&
    CommandBuffer::operator=(CommandBuffer&& commandBuffer) noexcept
    {
        ND_SET_SCOPE();

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
        ND_SET_SCOPE();

        vkFreeCommandBuffers(device_, commandPool_, 1, &commandBuffer_);
    }

    VkCommandBufferAllocateInfo
    getCommandBufferAllocateInfo(const VkCommandPool        commandPool,
                                 const VkCommandBufferLevel level,
                                 const uint32_t             commandBufferCount,
                                 const void*                next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, // sType;
            next,                                           // pNext;
            commandPool,                                    // commandPool;
            level,                                          // level;
            commandBufferCount                              // commandBufferCount;
        };
    }
} // namespace nd::src::graphics::vulkan
