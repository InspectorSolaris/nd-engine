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

    template<typename T, typename Filter, typename Collection = std::vector<T>>
    Collection
    getFiltered(const Collection& ts, const Filter& filter) noexcept
    {
        auto filtered = Collection {};

        for(size_t index = 0; index < ts.size(); ++index)
        {
            const auto t = ts[index];

            if(filter(t, index))
            {
                filtered.push_back(t);
            }
        }

        return filtered;
    }

    template<typename Out, typename Map, typename CollectionOut = std::vector<Out>>
    CollectionOut
    getMapped(const size_t size, const Map& map) noexcept
    {
        auto outs = CollectionOut {};

        outs.reserve(size);

        for(size_t index = 0; index < size; ++index)
        {
            outs.push_back(map(index));
        }

        return outs;
    }

    template<typename In,
             typename Out,
             typename Map,
             typename CollectionIn  = std::vector<In>,
             typename CollectionOut = std::vector<Out>>
    CollectionOut
    getMapped(const CollectionIn& ins, const Map& map) noexcept
    {
        auto outs = CollectionOut {};

        outs.reserve(ins.size());

        for(size_t index = 0; index < ins.size(); ++index)
        {
            outs.push_back(map(ins[index], index));
        }

        return outs;
    }

    template<typename T,
             typename Key,
             typename Value,
             typename KeyMap,
             typename ValueMap,
             typename Collection = std::vector<T>>
    std::map<Key, Value>
    getMap(const Collection& ts, const KeyMap& keyMap, const ValueMap& valueMap) noexcept
    {
        auto map = std::map<Key, Value> {};

        for(size_t index = 0; index < ts.size(); ++index)
        {
            const auto& t = ts[index];

            map[keyMap(t, index)] = valueMap(t, index);
        }

        return map;
    }
} // namespace nd::src::graphics::vulkan
