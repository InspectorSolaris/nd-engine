#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    class CommandPool final
    {
    public:
        CommandPool() noexcept;
        CommandPool(const VkDevice device, const VkCommandPoolCreateInfo& createInfo);

        CommandPool(const CommandPool& commandPool) = delete;
        CommandPool(CommandPool&& commandPool) noexcept;

        CommandPool&
        operator=(const CommandPool& commandPool) = delete;
        CommandPool&
        operator=(CommandPool&& commandPool) noexcept;

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

    VkCommandPoolCreateInfo
    getCommandPoolCreateInfo(const uint32_t queueFamilyIndex) noexcept;
} // namespace nd::src::graphics::vulkan
