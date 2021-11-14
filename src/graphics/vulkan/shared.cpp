#include "shared.hpp"

namespace nd::src::graphics::vulkan
{
    bool
    isSubmask(const uint32_t mask, const uint32_t submask) noexcept
    {
        return (mask & submask) == submask;
    }

    uint32_t
    getNextBit(const uint32_t bits) noexcept
    {
        return bits - (bits & (bits - 1));
    }

    std::vector<std::string>
    getMerged(const std::vector<std::string> &data1, const std::vector<std::string> &data2) noexcept
    {
        auto dataMerged = std::vector<std::string>(data1.begin(), data1.end());

        for (const auto &d2 : data2)
        {
            if (std::none_of(dataMerged.begin(), dataMerged.end(), [&d2](const auto &dm) { return dm == d2; }))
            {
                dataMerged.push_back(d2);
            }
        }

        return dataMerged;
    }

    std::vector<const char *>
    getCStrings(const std::vector<std::string> &data) noexcept
    {
        auto dataCStrings = std::vector<const char *>(data.size());

        std::transform(data.begin(), data.end(), dataCStrings.begin(), [](const auto &str) { return str.c_str(); });

        return dataCStrings;
    }
}    // namespace nd::src::graphics::vulkan
