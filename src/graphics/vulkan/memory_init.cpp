#include "memory_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    u8
    getMemoryTypeIndex(opt<const DeviceMemoryCfg>::ref cfg, const VkPhysicalDeviceMemoryProperties& memoryProperties) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        for(u8 index = 0; index < memoryProperties.memoryTypeCount; ++index)
        {
            const auto memoryType = memoryProperties.memoryTypes[index];
            const auto memoryHeap = memoryProperties.memoryHeaps[memoryType.heapIndex];

            if(isContainsAll(memoryType.propertyFlags, cfg.propertyFlags) && !isContainsAny(memoryType.propertyFlags, cfg.propertyFlagsNot) &&
               memoryHeap.size > cfg.size)
            {
                return index;
            }
        }

        ND_ASSERT_STATIC();
    }

    DeviceMemory
    allocateMemory(opt<const DeviceMemoryCfg>::ref         cfg,
                   const VkPhysicalDeviceMemoryProperties& memoryProperties,
                   const VkDevice                          device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto memoryTypeIndex = getMemoryTypeIndex(cfg, memoryProperties);

        const auto allocateInfo = VkMemoryAllocateInfo {.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                                                        .pNext           = cfg.next,
                                                        .allocationSize  = cfg.size,
                                                        .memoryTypeIndex = memoryTypeIndex};

        VkDeviceMemory deviceMemory;

        ND_ASSERT_EXEC(vkAllocateMemory(device, &allocateInfo, ND_VULKAN_ALLOCATION_CALLBACKS, &deviceMemory) == VK_SUCCESS);

        return {cfg.size, deviceMemory};
    }
} // namespace nd::src::graphics::vulkan
