#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    class CommandBuffer final
    {
    public:
        using AllocateInfo = VkCommandBufferAllocateInfo;

        CommandBuffer() noexcept;
        CommandBuffer(const VkDevice device, const VkCommandPool commandPool, const AllocateInfo& allocateInfo);

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

    CommandBuffer::AllocateInfo
    getCommandBufferAllocateInfo(const VkCommandPool        commandPool,
                                 const VkCommandBufferLevel level,
                                 const uint32_t             commandBufferCount) noexcept;
} // namespace nd::src::graphics::vulkan
