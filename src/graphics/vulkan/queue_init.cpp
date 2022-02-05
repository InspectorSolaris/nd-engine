#include "queue_init.hpp"
#include "tools_runtime.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VkQueue
    getQueue(const VkDevice device, const u32 queueFamilyIndex, const u32 queueIndex) noexcept
    {
        ND_SET_SCOPE();

        VkQueue queue;

        vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, &queue);

        return queue;
    }

    QueueFamily
    getQueueFamily(opt<const QueueFamilyCfg>::ref cfg, const vec<QueueFamily>& queueFamilies) noexcept(ND_ASSERT_NOTHROW)
    {
        ND_SET_SCOPE();

        const auto queueFamily = std::find_if(queueFamilies.begin(),
                                              queueFamilies.end(),
                                              [&cfg](const auto queueFamily)
                                              {
                                                  return isContainsAll(queueFamily.queueFlags, cfg.queueFlags) &&
                                                      !isContainsAny(queueFamily.queueFlags, cfg.queueFlagsNot);
                                              });

        ND_ASSERT(queueFamily != queueFamilies.end());

        return *queueFamily;
    }
} // namespace nd::src::graphics::vulkan
