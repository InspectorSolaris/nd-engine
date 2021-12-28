#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    VkSemaphoreCreateInfo
    getSemaphoreCreateInfo(const VkSemaphoreCreateFlags flags = {}, const void* next = {}) noexcept;

    VkSemaphore
    getSemaphore(const VkSemaphoreCreateInfo& createInfo, const VkDevice device);

    VkSemaphore
    getSemaphore(const VkDevice device, const VkSemaphoreCreateFlags flags = {}, const void* next = {});

    std::vector<VkSemaphore>
    getSemaphore(const VkDevice device, const size_t count, const VkSemaphoreCreateFlags flags = {}, const void* next = {});

    VkFenceCreateInfo
    getFenceCreateInfo(const VkFenceCreateFlags flags = {}, const void* next = {}) noexcept;

    VkFence
    getFence(const VkFenceCreateInfo& createInfo, const VkDevice device);

    VkFence
    getFence(const VkDevice device, const VkFenceCreateFlags flags = {}, const void* next = {});

    std::vector<VkFence>
    getFence(const VkDevice device, const size_t count, const VkFenceCreateFlags flags = {}, const void* next = {});
} // namespace nd::src::graphics::vulkan
