#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class CommandBuffer final
    {
    public:
        CommandBuffer() noexcept;
        CommandBuffer(const VkDevice                     device,
                      const VkCommandPool                commandPool,
                      const VkCommandBufferAllocateInfo& allocateInfo);

        CommandBuffer(const CommandBuffer& commandBuffer) = delete;
        CommandBuffer(CommandBuffer&& commandBuffer) noexcept;

        CommandBuffer&
        operator=(const CommandBuffer& commandBuffer) = delete;
        CommandBuffer&
        operator=(CommandBuffer&& commandBuffer) noexcept;

        ~CommandBuffer();

        constexpr VkCommandBuffer
        get() const noexcept;

    private:
        VkDevice        device_ {VK_NULL_HANDLE};
        VkCommandPool   commandPool_ {VK_NULL_HANDLE};
        VkCommandBuffer commandBuffer_ {VK_NULL_HANDLE};
    };

    constexpr VkCommandBuffer
    CommandBuffer::get() const noexcept
    {
        return commandBuffer_;
    }

    VkCommandBufferAllocateInfo
    getCommandBufferAllocateInfo(const VkCommandPool        commandPool,
                                 const VkCommandBufferLevel level,
                                 const uint32_t             commandBufferCount,
                                 const void*                next = {}) noexcept;
} // namespace nd::src::graphics::vulkan
