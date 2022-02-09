#include "command_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    void
    resetCommandPools(gsl::span<const CommandPool> commandPools, const VkDevice device, const VkCommandPoolResetFlags flags) noexcept
    {
        ND_SET_SCOPE();

        for(opt<const CommandPool>::ref commandPool: commandPools)
        {
            vkResetCommandPool(device, commandPool, flags);
        }
    }

    CommandPool
    createCommandPool(opt<const CommandPoolCfg>::ref cfg, const VkDevice device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto createInfo = VkCommandPoolCreateInfo {.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                                                         .pNext            = cfg.next,
                                                         .flags            = cfg.flags,
                                                         .queueFamilyIndex = static_cast<u32>(cfg.queueFamily.index)};

        VkCommandPool commandPool;

        ND_ASSERT_EXEC(vkCreateCommandPool(device, &createInfo, ND_VULKAN_ALLOCATION_CALLBACKS, &commandPool) == VK_SUCCESS);

        return commandPool;
    }

    vec<CommandPool>
    createCommandPools(const vec<CommandPoolCfg>& cfg, const VkDevice device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return getMapped<CommandPoolCfg, CommandPool>(cfg,
                                                      [device](const auto& cfg, const auto index)
                                                      {
                                                          return createCommandPool(cfg, device);
                                                      });
    }

    CommandPoolObjects
    createCommandPoolObjects(opt<const CommandPoolObjectsCfg>::ref cfg, const VkDevice device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.graphics = createCommandPools(cfg.graphics, device),
                .transfer = createCommandPools(cfg.transfer, device),
                .compute  = createCommandPools(cfg.compute, device)};
    }

    vec<CommandBuffer>
    allocateCommandBuffers(opt<const CommandBufferCfg>::ref cfg,
                           opt<const CommandPool>::ref      commandPool,
                           const VkDevice                   device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto allocateInfo = VkCommandBufferAllocateInfo {.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                                                               .pNext              = cfg.next,
                                                               .commandPool        = commandPool,
                                                               .level              = cfg.level,
                                                               .commandBufferCount = static_cast<u32>(cfg.count)};

        auto commandBuffers = vec<CommandBuffer>(cfg.count);

        ND_ASSERT_EXEC(vkAllocateCommandBuffers(device, &allocateInfo, commandBuffers.data()) == VK_SUCCESS);

        return commandBuffers;
    }
} // namespace nd::src::graphics::vulkan
