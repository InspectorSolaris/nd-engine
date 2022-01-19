#include "tools.hpp"

namespace nd::src::tools
{
    std::runtime_error
    getRuntimeError(const std::string_view file, const std::string_view function, const std::string_view reason, const size_t line) noexcept
    {
        return std::runtime_error(fmt::format("{}:{} ({}) {}", file, line, function, reason));
    }

    bool
    isSubmask(const int mask, const int submask) noexcept
    {
        return (mask & submask) == submask;
    }

    bool
    isNotSubmask(const int mask, const int submask) noexcept
    {
        return !(mask & submask);
    }

    int
    getBitIndex(const int bit) noexcept
    {
        auto index = 0;

        while(index < 32 && (bit & (1 << index)) == 0)
        {
            ++index;
        }

        return index;
    }

    int
    getNextBit(const int bits) noexcept
    {
        return bits - (bits & (bits - 1));
    }

    int
    getNextBitIndex(const int bits) noexcept
    {
        return getBitIndex(getNextBit(bits));
    }

    std::vector<std::string>
    getMerged(const std::vector<std::string>& strings1, const std::vector<std::string>& strings2) noexcept
    {
        auto strings = strings1;

        for(const auto& s2: strings2)
        {
            if(std::none_of(strings.begin(),
                            strings.end(),
                            [&s2](const auto& s)
                            {
                                return s == s2;
                            }))
            {
                strings.push_back(s2);
            }
        }

        return strings;
    }

    std::vector<const char*>
    getRawStrings(const std::vector<std::string>& strings) noexcept
    {
        return getMapped<std::string, const char*>(strings,
                                                   [](const auto& string, const auto index)
                                                   {
                                                       return string.c_str();
                                                   });
    }
} // namespace nd::src::tools
