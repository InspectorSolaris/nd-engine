#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    bool
    isSubmask(const uint32_t mask, const uint32_t submask) noexcept;

    uint32_t
    getNextBit(const uint32_t bits) noexcept;

    std::vector<std::string>
    getMerged(const std::vector<std::string>& data1, const std::vector<std::string>& data2) noexcept;

    std::vector<const char*>
    getCStrings(const std::vector<std::string>& data) noexcept;

    template<typename A,
             typename B,
             typename Map,
             typename CollectionA = std::vector<A>,
             typename CollectionB = std::vector<B>>
    CollectionB
    getMapped(const CollectionA& as, const Map& map) noexcept
    {
        auto bs = CollectionB(as.size());

        std::transform(as.begin(), as.end(), bs.begin(), map);

        return bs;
    }
} // namespace nd::src::graphics::vulkan
