#include "tools_runtime.hpp"

namespace nd::src::tools
{
    std::runtime_error
    getRuntimeError(const str_v file, const str_v function, const str_v reason, const u64 line) noexcept
    {
        return std::runtime_error(fmt::format("{}:{} ({}) {}", file, line, function, reason));
    }

    bool
    isContainsAll(const u64 mask, const u64 submask) noexcept
    {
        return (mask & submask) == submask;
    }

    bool
    isContainsAny(const u64 mask, const u64 submask) noexcept
    {
        return mask & submask;
    }

    u64
    getNextBit(const u64 bits) noexcept
    {
        return bits - (bits & (bits - 1));
    }

    u8
    getNextBitIndex(const u64 bits) noexcept
    {
        return getBitIndex(getNextBit(bits));
    }

    u8
    getBitIndex(const u64 bit) noexcept
    {
        auto index = 0;

        while(index < sizeof(bit) && (bit & (1 << index)) == 0)
        {
            ++index;
        }

        return index;
    }

    vec<str>
    getMerged(const vec<str>& strs1, const vec<str>& strs2) noexcept
    {
        auto merged = strs1;

        for(const auto& s2: strs2)
        {
            if(std::none_of(merged.begin(),
                            merged.end(),
                            [&s2](const auto& s)
                            {
                                return s == s2;
                            }))
            {
                merged.push_back(s2);
            }
        }

        return merged;
    }

    vec<const char*>
    getRawStrings(const vec<str>& strs) noexcept
    {
        return getMapped<str, const char*>(strs,
                                           [](const auto& str, const auto index)
                                           {
                                               return str.c_str();
                                           });
    }
} // namespace nd::src::tools
