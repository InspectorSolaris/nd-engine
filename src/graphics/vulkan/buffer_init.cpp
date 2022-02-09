#include "buffer_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    BufferMesh
    createBufferMesh(opt<const BufferMeshCfg>::ref cfg, const VkDevice device, const VkPhysicalDevice physicalDevice) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto size = cfg.vertex.size + cfg.index.size + cfg.uniform.size;

        const auto createInfo = VkBufferCreateInfo {
            .sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext                 = cfg.next,
            .flags                 = cfg.flags,
            .size                  = size,
            .usage                 = cfg.usage,
            .sharingMode           = cfg.queueFamilyIndices.size() <= 1 ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
            .queueFamilyIndexCount = static_cast<u32>(cfg.queueFamilyIndices.size()),
            .pQueueFamilyIndices   = cfg.queueFamilyIndices.data()};

        VkBuffer buffer;

        ND_ASSERT_EXEC(vkCreateBuffer(device, &createInfo, ND_VULKAN_ALLOCATION_CALLBACKS, &buffer) == VK_SUCCESS);

        const auto offset = bindBufferMemory(buffer, cfg.memory, device, physicalDevice) - cfg.vertex.offset;

        return {.vertex  = {.offset = cfg.vertex.offset + offset, .size = cfg.vertex.size},
                .index   = {.offset = cfg.index.offset + offset, .size = cfg.index.size},
                .uniform = {.offset = cfg.uniform.offset + offset, .size = cfg.uniform.size},
                .memory  = cfg.memory.handle,
                .handle  = buffer};
    }

    BufferStage
    createBufferStage(opt<const BufferStageCfg>::ref cfg, const VkDevice device, const VkPhysicalDevice physicalDevice) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto size = cfg.range.size;

        const auto createInfo = VkBufferCreateInfo {
            .sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext                 = cfg.next,
            .flags                 = cfg.flags,
            .size                  = size,
            .usage                 = cfg.usage,
            .sharingMode           = cfg.queueFamilyIndices.size() <= 1 ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
            .queueFamilyIndexCount = static_cast<u32>(cfg.queueFamilyIndices.size()),
            .pQueueFamilyIndices   = cfg.queueFamilyIndices.data()};

        VkBuffer buffer;

        ND_ASSERT_EXEC(vkCreateBuffer(device, &createInfo, ND_VULKAN_ALLOCATION_CALLBACKS, &buffer) == VK_SUCCESS);

        const auto offset = bindBufferMemory(buffer, cfg.memory, device, physicalDevice) - cfg.range.offset;

        return {.range = {.offset = cfg.range.offset + offset, .size = cfg.range.size}, .memory = cfg.memory.handle, .handle = buffer};
    }

    BufferObjects
    createBufferObjects(opt<const BufferObjectsCfg>::ref cfg,
                        const VkDevice                   device,
                        const VkPhysicalDevice           physicalDevice) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        return {.mesh = createBufferMesh(cfg.mesh, device, physicalDevice), .stage = createBufferStage(cfg.stage, device, physicalDevice)};
    }
} // namespace nd::src::graphics::vulkan
