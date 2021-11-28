#include "queue.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    std::vector<uint32_t>
    getQueueFamiliesIndices(const std::vector<const QueueFamily*>& queueFamilies) noexcept
    {
        ND_SET_SCOPE_LOW();

        auto queueFamiliesIndices = std::vector<uint32_t>(queueFamilies.size());

        std::transform(queueFamilies.begin(),
                       queueFamilies.end(),
                       queueFamiliesIndices.begin(),
                       [](const auto& swapchainQueueFamily)
                       {
                           return swapchainQueueFamily->index;
                       });

        return queueFamiliesIndices;
    }

} // namespace nd::src::graphics::vulkan
