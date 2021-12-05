#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    bool
    isSubmask(const uint32_t mask, const uint32_t submask) noexcept;

    uint32_t
    getNextBit(const uint32_t bits) noexcept;

    std::vector<std::string>
    getMerged(const std::vector<std::string>& strings1, const std::vector<std::string>& strings2) noexcept;

    std::vector<const char*>
    getRawStrings(const std::vector<std::string>& strings) noexcept;

    template<typename A,
             typename B,
             typename Map,
             typename CollectionA = std::vector<A>,
             typename CollectionB = std::vector<B>>
    CollectionB
    getMapped(const CollectionA& as, const Map& map) noexcept
    {
        auto bs = CollectionB {};

        bs.reserve(as.size());

        for(const auto& a: as)
        {
            bs.push_back(map(a));
        }

        return bs;
    }
} // namespace nd::src::graphics::vulkan
