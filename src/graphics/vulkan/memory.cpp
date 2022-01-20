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

    std::vector<size_t>
    bindBufferMemory(const VkDevice                          device,
                     const VkPhysicalDeviceMemoryProperties* memoryProperties,
                     const std::vector<DeviceMemory>&        deviceMemories,
                     const std::vector<Buffer>&              buffers)
    {
        ND_SET_SCOPE();

        auto memoryIndices = std::vector<size_t>(buffers.size());

        for(auto index = 0; index < buffers.size(); ++index)
        {
            const auto& buffer       = buffers[index];
            const auto  requirements = getMemoryRequirements(device, buffer.handle);

            auto typeIndex = std::optional<uint32_t> {};
            auto typeBits  = requirements.memoryTypeBits;

            while(typeBits)
            {
                const auto typeBitIndex = getNextBitIndex(typeBits);
                const auto flags        = memoryProperties->memoryTypes[typeBitIndex].propertyFlags;

                if(isSubmask(flags, buffer.memoryPropertyFlags) && isNotSubmask(flags, buffer.memoryPropertyNotFlags))
                {
                    typeIndex = typeBitIndex;

                    break;
                }

                typeBits -= (1 << typeBitIndex);
            }

            ND_ASSERT(typeIndex.has_value());

            const auto deviceMemory = std::find_if(deviceMemories.begin(),
                                                   deviceMemories.end(),
                                                   [typeIndex](const auto& deviceMemory)
                                                   {
                                                       return deviceMemory.memoryTypeIndex == typeIndex.value();
                                                   });

            ND_ASSERT(deviceMemory != deviceMemories.end());

            vkBindBufferMemory(device, buffer.handle, deviceMemory->handle, getAlignedOffset(0, requirements.alignment));

            memoryIndices[index] = std::distance(deviceMemories.begin(), deviceMemory);
        }

        return memoryIndices;
    }

    std::vector<size_t>
    bindImageMemory(const VkDevice                          device,
                    const VkPhysicalDeviceMemoryProperties* memoryProperties,
                    const std::vector<DeviceMemory>&        deviceMemories,
                    const std::vector<Image>&               images)
    {
        ND_SET_SCOPE();

        return std::vector<size_t> {};
    }

    VkMemoryRequirements
    getMemoryRequirements(const VkDevice device, const VkBuffer buffer) noexcept
    {
        ND_SET_SCOPE();

        VkMemoryRequirements memoryRequirements;

        vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

        return memoryRequirements;
    }

    VkMemoryRequirements
    getMemoryRequirements(const VkDevice device, const VkImage image) noexcept
    {
        ND_SET_SCOPE();

        VkMemoryRequirements memoryRequirements;

        vkGetImageMemoryRequirements(device, image, &memoryRequirements);

        return memoryRequirements;
    }

    uint32_t
    getMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties* memoryProperties,
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

            if(isSubmask(memoryProperties->memoryTypes[index].propertyFlags, flags))
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

        const auto allocateInfo = getMemoryAllocateInfo(configuration.size, configuration.memoryTypeIndex, configuration.next);

        return {getDeviceMemoryHandle(allocateInfo, device), configuration.size, configuration.memoryTypeIndex};
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
} // namespace nd::src::graphics::vulkan
