#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    class CommandPool final
    {
    public:
        using CreateInfo = VkCommandPoolCreateInfo;

        CommandPool() noexcept;
        CommandPool(const VkDevice device, const CreateInfo &createInfo);

        CommandPool(const CommandPool &commandPool) = delete;
        CommandPool(CommandPool &&commandPool) noexcept;

        CommandPool &
        operator=(const CommandPool &commandPool) = delete;
        CommandPool &
        operator=(CommandPool &&commandPool) noexcept;

        ~CommandPool();

        constexpr VkCommandPool
        get() const noexcept;

    private:
        VkDevice      device_ {VK_NULL_HANDLE};
        VkCommandPool commandPool_ {VK_NULL_HANDLE};
    };

    constexpr VkCommandPool
    CommandPool::get() const noexcept
    {
        return commandPool_;
    }

    CommandPool::CreateInfo
    getCommandPoolCreateInfo(const uint32_t queueFamilyIndex) noexcept;
}    // namespace nd::src::graphics::vulkan
