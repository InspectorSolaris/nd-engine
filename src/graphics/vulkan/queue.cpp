#include "queue.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    std::vector<uint32_t>
    getQueueFamiliesIndices(const std::vector<const QueueFamily*>& queueFamilies) noexcept
    {
        ND_SET_SCOPE_LOW();

        return getMapped<const QueueFamily*, uint32_t>(queueFamilies,
                                                       [](const auto& queueFamily)
                                                       {
                                                           return queueFamily->index;
                                                       });
    }

} // namespace nd::src::graphics::vulkan
