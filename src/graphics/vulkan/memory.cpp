#include "memory.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VkDeviceSize
    getAlignedOffset(const VkDeviceSize offset, const VkDeviceSize alignment) noexcept
    {
        ND_SET_SCOPE();

        const auto mod = offset % alignment;

        if(!mod)
        {
            return offset;
        }

        return offset + alignment - mod;
    }

    void
    setMemory(const VkDevice device, const DeviceMemory memory, const VkDeviceSize offset, const void* data) noexcept
    {
        ND_SET_SCOPE();

        void* ptr;

        vkMapMemory(device, memory.handle, offset, memory.size, {}, &ptr);

        memcpy(ptr, data, memory.size);

        vkUnmapMemory(device, memory.handle);
    }

    void
    bindMemories(const VkDevice                          device,
                 const std::vector<Buffer>&              buffers,
                 const std::vector<DeviceMemories>&      bufferMemories,
                 const VkPhysicalDeviceMemoryProperties& physicalDeviceMemoryProperties) noexcept
    {
        ND_SET_SCOPE();

        VkDeviceSize offsets[VK_MAX_MEMORY_HEAPS] = {};

        for(size_t bufferIndex = 0; bufferIndex < buffers.size(); ++bufferIndex)
        {
            for(size_t memoryIndex = 0; memoryIndex < bufferMemories[bufferIndex].size(); ++memoryIndex)
            {
                const auto memory          = bufferMemories[bufferIndex][memoryIndex];
                const auto memoryHeapIndex = physicalDeviceMemoryProperties.memoryTypes[memory.memoryTypeIndex].heapIndex;

                const auto offset = getAlignedOffset(offsets[memoryHeapIndex], memory.alignment);

                vkBindBufferMemory(device, buffers[bufferIndex], memory.handle, offset);

                offsets[memoryHeapIndex] = offset + memory.size;
            }
        }
    }

    VkMemoryRequirements
    getMemoryRequirements(const VkDevice device, const VkBuffer buffer) noexcept
    {
        ND_SET_SCOPE();

        VkMemoryRequirements memoryRequirements;

        vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

        return memoryRequirements;
    }

    uint32_t
    getMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties& memoryProperties,
                       const VkMemoryRequirements              memoryRequirements,
                       const VkMemoryPropertyFlags             flags)
    {
        ND_SET_SCOPE();

        auto memoryTypeBits  = memoryRequirements.memoryTypeBits;
        auto memoryTypeIndex = std::optional<uint32_t> {};

        while(memoryTypeBits)
        {
            const auto bit   = getNextBit(memoryTypeBits);
            const auto index = getBitIndex(bit);

            if(isSubmask(memoryProperties.memoryTypes[index].propertyFlags, flags))
            {
                memoryTypeIndex = index;
            }

            memoryTypeBits -= bit;
        }

        ND_ASSERT(memoryTypeIndex.has_value());

        return memoryTypeIndex.value();
    }

    VkMemoryAllocateInfo
    getMemoryAllocateInfo(const VkDeviceSize allocationSize, const uint32_t memoryTypeIndex, const void* next) noexcept
    {
        ND_SET_SCOPE();

        return {
            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, // sType;
            next,                                   // pNext;
            allocationSize,                         // allocationSize;
            memoryTypeIndex                         // memoryTypeIndex;
        };
    }

    VkDeviceMemory
    getDeviceMemoryHandle(const VkMemoryAllocateInfo& allocateInfo, const VkDevice device)
    {
        ND_SET_SCOPE();

        VkDeviceMemory deviceMemory;

        ND_ASSERT_EXEC(vkAllocateMemory(device, &allocateInfo, nullptr, &deviceMemory) == VK_SUCCESS);

        return deviceMemory;
    }

    DeviceMemory
    getDeviceMemory(const DeviceMemoryConfiguration& configuration, const VkDevice device)
    {
        ND_SET_SCOPE();

        const auto allocateInfo = getMemoryAllocateInfo(configuration.allocationSize, configuration.memoryTypeIndex, configuration.next);

        return {getDeviceMemoryHandle(allocateInfo, device), configuration.allocationSize, configuration.alignment, configuration.memoryTypeIndex};
    }

    std::vector<DeviceMemory>
    getDeviceMemories(const std::vector<DeviceMemoryConfiguration>& configurations, const VkDevice device)
    {
        ND_SET_SCOPE();

        return getMapped<DeviceMemoryConfiguration, DeviceMemory>(configurations,
                                                                  [device](const auto configuration, const auto index)
                                                                  {
                                                                      return getDeviceMemory(configuration, device);
                                                                  });
    }

    std::vector<DeviceMemories>
    getAllDeviceMemories(const std::vector<std::vector<DeviceMemoryConfiguration>>& configurations, const VkDevice device)
    {
        ND_SET_SCOPE();

        return getMapped<std::vector<DeviceMemoryConfiguration>, std::vector<DeviceMemory>>(configurations,
                                                                                            [device](const auto& bufferMemoryConfig, const auto index)
                                                                                            {
                                                                                                return getDeviceMemories(bufferMemoryConfig, device);
                                                                                            });
    }
} // namespace nd::src::graphics::vulkan
