#include "command_pool.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    CommandPool::CommandPool() noexcept
    {
        ND_SET_SCOPE_LOW();
    }

    CommandPool::CommandPool(const VkDevice device, const CreateInfo& createInfo)
        : device_(device)
    {
        ND_SET_SCOPE_LOW();

        ND_ASSERT(vkCreateCommandPool(device_, &createInfo, nullptr, &commandPool_) == VK_SUCCESS);
    }

    CommandPool::CommandPool(CommandPool&& commandPool) noexcept
        : device_(std::move(commandPool.device_))
        , commandPool_(std::move(commandPool.commandPool_))
    {
        ND_SET_SCOPE_LOW();

        commandPool.commandPool_ = VK_NULL_HANDLE;
    }

    CommandPool&
    CommandPool::operator=(CommandPool&& commandPool) noexcept
    {
        ND_SET_SCOPE_LOW();

        if(&commandPool == this)
        {
            return *this;
        }

        device_      = std::move(commandPool.device_);
        commandPool_ = std::move(commandPool.commandPool_);

        commandPool.commandPool_ = VK_NULL_HANDLE;

        return *this;
    }

    CommandPool::~CommandPool()
    {
        ND_SET_SCOPE_LOW();

        vkDestroyCommandPool(device_, commandPool_, nullptr);
    }

    CommandPool::CreateInfo
    getCommandPoolCreateInfo(const uint32_t queueFamilyIndex) noexcept
    {
        ND_SET_SCOPE_LOW();

        return {
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, // sType;
            nullptr,                                    // pNext;
            0,                                          // flags;
            queueFamilyIndex                            // queueFamilyIndex;
        };
    }
} // namespace nd::src::graphics::vulkan
