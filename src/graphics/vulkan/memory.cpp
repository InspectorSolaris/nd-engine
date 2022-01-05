#include "memory.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VkPhysicalDeviceMemoryProperties
    getPhysicalDeviceMemoryProperties(const VkPhysicalDevice physicalDevice) noexcept
    {
        ND_SET_SCOPE();

        VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;

        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);

        return physicalDeviceMemoryProperties;
    }

    VkMemoryRequirements
    getMemoryRequirements(const VkDevice device, const VkBuffer buffer) noexcept
    {
        ND_SET_SCOPE();

        VkMemoryRequirements memoryRequirements;

        vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

        return memoryRequirements;
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

        return getDeviceMemoryHandle(allocateInfo, device);
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
