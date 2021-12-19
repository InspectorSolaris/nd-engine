#pragma once

#include "pch.hpp"
#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    VkSemaphoreCreateInfo
    getSemaphoreCreateInfo(const VkSemaphoreCreateFlags flags = {}, const void* next = {}) noexcept;

    VkSemaphore
    getSemaphore(const VkSemaphoreCreateInfo& createInfo, const VkDevice device);

    VkSemaphore
    getSemaphore(const VkDevice device);

    VkFenceCreateInfo
    getFenceCreateInfo(const VkFenceCreateFlags flags = {}, const void* next = {}) noexcept;

    VkFence
    getFence(const VkFenceCreateInfo& createInfo, const VkDevice device);

    VkFence
    getFence(const VkDevice device);
} // namespace nd::src::graphics::vulkan
