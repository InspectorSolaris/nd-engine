#include "memory_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;



    struct MemoryState final
    {
        vec<VkDeviceSize> offsets;
        vec<VkDeviceSize> size;
    };

    MemoryState&
    getMemoryState(const VkPhysicalDevice physicalDevice) noexcept
    {
        ND_SET_SCOPE();

        static auto memoryStates = map<VkPhysicalDevice, MemoryState> {};

        if(memoryStates.find(physicalDevice) == memoryStates.end())
        {
            const auto memoryProperties = getMemoryProperties(physicalDevice);

            auto memoryState = MemoryState {.offsets = vec<VkDeviceSize>(memoryProperties.memoryHeapCount, 0ULL),
                                            .size    = vec<VkDeviceSize>(memoryProperties.memoryHeapCount)};

            for(u8 index = 0; index < memoryProperties.memoryHeapCount; ++index)
            {
                memoryState.size[index] = memoryProperties.memoryHeaps[index].size;
            }

            memoryStates[physicalDevice] = std::move(memoryState);
        }

        return memoryStates[physicalDevice];
    }



    VkPhysicalDeviceMemoryProperties
    getMemoryProperties(const VkPhysicalDevice physicalDevice) noexcept
    {
        ND_SET_SCOPE();

        VkPhysicalDeviceMemoryProperties properties;

        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &properties);

        return properties;
    }

    VkMemoryRequirements
    getBufferMemoryRequirements(const VkBuffer buffer, const VkDevice device) noexcept
    {
        ND_SET_SCOPE();

        VkMemoryRequirements requirements;

        vkGetBufferMemoryRequirements(device, buffer, &requirements);

        return requirements;
    }

    VkMemoryRequirements
    getImageMemoryRequirements(const VkImage image, const VkDevice device) noexcept
    {
        ND_SET_SCOPE();

        VkMemoryRequirements requirements;

        vkGetImageMemoryRequirements(device, image, &requirements);

        return requirements;
    }

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

    VkDeviceSize
    getMemoryOffsetAligned(const VkDeviceSize offset, const VkDeviceSize alignment) noexcept
    {
        ND_SET_SCOPE();

        const auto mod = offset % alignment;

        if(!mod)
        {
            return offset;
        }

        return offset + alignment - mod;
    }

    VkDeviceSize
    getMemoryOffset(opt<const DeviceMemory>::ref memory,
                    const VkMemoryRequirements&  requirements,
                    const VkPhysicalDevice       physicalDevice) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto& memoryState = getMemoryState(physicalDevice);

        ND_ASSERT(isContainsAny(requirements.memoryTypeBits, 1 << memory.typeIndex));
        ND_ASSERT(requirements.size <= memoryState.size[memory.heapIndex] - memoryState.offsets[memory.heapIndex]);

        return getMemoryOffsetAligned(memoryState.offsets[memory.heapIndex], requirements.alignment);
    }

    DeviceMemory
    allocateMemory(opt<const DeviceMemoryCfg>::ref         cfg,
                   const VkPhysicalDeviceMemoryProperties& memoryProperties,
                   const VkDevice                          device) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto memoryTypeIndex = getMemoryTypeIndex(cfg, memoryProperties);
        const auto memoryHeapIndex = static_cast<u8>(memoryProperties.memoryTypes[memoryTypeIndex].heapIndex);

        const auto allocateInfo = VkMemoryAllocateInfo {.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                                                        .pNext           = cfg.next,
                                                        .allocationSize  = cfg.size,
                                                        .memoryTypeIndex = memoryTypeIndex};

        VkDeviceMemory deviceMemory;

        ND_ASSERT_EXEC(vkAllocateMemory(device, &allocateInfo, ND_VULKAN_ALLOCATION_CALLBACKS, &deviceMemory) == VK_SUCCESS);

        return {.handle = deviceMemory, .typeIndex = memoryTypeIndex, .heapIndex = memoryHeapIndex};
    }

    VkDeviceSize
    bindBufferMemory(const VkBuffer               buffer,
                     opt<const DeviceMemory>::ref memory,
                     const VkDevice               device,
                     const VkPhysicalDevice       physicalDevice) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto requirements = getBufferMemoryRequirements(buffer, device);
        const auto offset       = getMemoryOffset(memory, requirements, physicalDevice);

        ND_ASSERT_EXEC(vkBindBufferMemory(device, buffer, memory.handle, offset) == VK_SUCCESS);

        return getMemoryState(physicalDevice).offsets[memory.heapIndex] = offset;
    }

    VkDeviceSize
    bindImageMemory(const VkImage                image,
                    opt<const DeviceMemory>::ref memory,
                    const VkDevice               device,
                    const VkPhysicalDevice       physicalDevice) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto requirements = getImageMemoryRequirements(image, device);
        const auto offset       = getMemoryOffset(memory, requirements, physicalDevice);

        ND_ASSERT_EXEC(vkBindImageMemory(device, image, memory.handle, offset) == VK_SUCCESS);

        return getMemoryState(physicalDevice).offsets[memory.heapIndex] = offset;
    }
} // namespace nd::src::graphics::vulkan
