#include "tools.hpp"

namespace nd::src::tools
{
    std::runtime_error
    getRuntimeError(const std::string_view file, const std::string_view function, const std::string_view reason, const size_t line) noexcept
    {
        return std::runtime_error(fmt::format("{}:{} ({}) {}", file, line, function, reason));
    }

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
