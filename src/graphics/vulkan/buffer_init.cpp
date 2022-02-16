#include "buffer_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    Buffer
    createBuffer(opt<const BufferCfg>::ref cfg, const VkDevice device, const VkPhysicalDevice physicalDevice) noexcept(ND_VK_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto createInfo = VkBufferCreateInfo {
            .sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext                 = cfg.next,
            .flags                 = cfg.flags,
            .size                  = cfg.size,
            .usage                 = cfg.usage,
            .sharingMode           = cfg.queueFamilyIndices.size() <= 1 ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
            .queueFamilyIndexCount = static_cast<u32>(cfg.queueFamilyIndices.size()),
            .pQueueFamilyIndices   = cfg.queueFamilyIndices.data()};

        VkBuffer buffer;

        ND_VK_ASSERT(vkCreateBuffer(device, &createInfo, ND_VK_ALLOCATION_CALLBACKS, &buffer));

        const auto offset = bindBufferMemory(buffer, cfg.memory, device, physicalDevice);

        return {.offset = offset, .handle = buffer};
    }

    BufferObjects
    createBufferObjects(opt<const BufferObjectsCfg>::ref cfg,
                        const VkDevice                   device,
                        const VkPhysicalDevice           physicalDevice) noexcept(ND_VK_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.mesh = createBuffer(cfg.mesh, device, physicalDevice), .stage = createBuffer(cfg.stage, device, physicalDevice)};
    }
} // namespace nd::src::graphics::vulkan
