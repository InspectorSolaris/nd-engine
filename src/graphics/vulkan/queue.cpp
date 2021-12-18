#include "queue.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    std::vector<uint32_t>
    getQueueFamiliesIndices(const std::vector<const QueueFamily*>& queueFamilies) noexcept
    {
        ND_SET_SCOPE();

        return getMapped<const QueueFamily*, uint32_t>(queueFamilies,
                                                       [](const auto& queueFamily, const auto index)
                                                       {
                                                           return queueFamily->index;
                                                       });
    }

} // namespace nd::src::graphics::vulkan
