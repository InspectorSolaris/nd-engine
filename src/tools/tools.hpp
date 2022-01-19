#pragma once

#include "pch.hpp"
#include "scope.hpp"

#if defined(__GNUC__)
    #define ND_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_BUILD)
    #define ND_FUNCTION __FUNCTION_SIG__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
    #define ND_FUNCTION __func__
#else
    #define ND_FUNCTION __func__
#endif

#define ND_USE_SCOPE        4
#define ND_USE_SCOPE_HIGH   3
#define ND_USE_SCOPE_MEDIUM 2
#define ND_USE_SCOPE_LOW    1

#define ND_USE_SCOPE_LEVEL 5

#if(ND_USE_SCOPE_LEVEL <= ND_USE_SCOPE)
    #define ND_SET_SCOPE() const auto scope = nd::src::tools::Scope(ND_FUNCTION, nd::src::tools::onScopeBegin, nd::src::tools::onScopeEnd)
#else
    #define ND_SET_SCOPE()
#endif

#if(ND_USE_SCOPE_LEVEL <= ND_USE_SCOPE_HIGH)
    #define ND_SET_SCOPE_HIGH() ND_SET_SCOPE()
#else
    #define ND_SET_SCOPE_HIGH()
#endif

#if(ND_USE_SCOPE_LEVEL <= ND_USE_SCOPE_MEDIUM)
    #define ND_SET_SCOPE_MEDIUM() ND_SET_SCOPE()
#else
    #define ND_SET_SCOPE_MEDIUM()
#endif

#if(ND_USE_SCOPE_LEVEL <= ND_USE_SCOPE_LOW)
    #define ND_SET_SCOPE_LOW() ND_SET_SCOPE()
#else
    #define ND_SET_SCOPE_LOW()
#endif

#define ND_GET_RUNTIME_ERROR(reason) nd::src::tools::getRuntimeError(__FILE__, ND_FUNCTION, #reason, __LINE__)

#if defined(NDEBUG)
    #define ND_ASSERT_EXEC(code) (code)
    #define ND_ASSERT(code)
#else
    #define ND_ASSERT_EXEC(code)              \
        if(!(code))                           \
        {                                     \
            throw ND_GET_RUNTIME_ERROR(code); \
        }
    #define ND_ASSERT(code) ND_ASSERT_EXEC(code)
#endif

namespace nd::src::tools
{
    const char* const logMainName  = "main";
    const char* const logScopeName = "scope";

    std::runtime_error
    getRuntimeError(const std::string_view file, const std::string_view function, const std::string_view reason, const size_t line) noexcept;

    bool
    isSubmask(const int mask, const int submask) noexcept;

    bool
    isNotSubmask(const int mask, const int submask) noexcept;

    int
    getBitIndex(const int bit) noexcept;

    int
    getNextBit(const int bits) noexcept;

    int
    getNextBitIndex(const int bits) noexcept;

    std::vector<std::string>
    getMerged(const std::vector<std::string>& strings1, const std::vector<std::string>& strings2) noexcept;

    std::vector<const char*>
    getRawStrings(const std::vector<std::string>& strings) noexcept;

    template<typename Type, typename FilterType, typename CollectionType = std::vector<Type>>
    CollectionType
    getFiltered(const CollectionType& ts, const FilterType& filter) noexcept
    {
        auto filtered = CollectionType {};

        for(auto index = 0; index < ts.size(); ++index)
        {
            const auto t = ts[index];

            if(filter(t, index))
            {
                filtered.push_back(t);
            }
        }

        return filtered;
    }

    template<typename Type, typename MapType, typename CollectionType = std::vector<Type>>
    CollectionType
    getMapped(const int size, const MapType& map) noexcept
    {
        auto mapped = CollectionType {};

        mapped.reserve(size);

        for(auto index = 0; index < size; ++index)
        {
            mapped.push_back(map(index));
        }

        return mapped;
    }

    template<typename TypeIn,
             typename Type,
             typename MapType,
             typename CollectionTypeIn = std::vector<TypeIn>,
             typename CollectionType   = std::vector<Type>>
    CollectionType
    getMapped(const CollectionTypeIn& ins, const MapType& map) noexcept
    {
        auto mapped = CollectionType {};

        mapped.reserve(ins.size());

        for(auto index = 0; index < ins.size(); ++index)
        {
            mapped.push_back(map(ins[index], index));
        }

        return mapped;
    }

    template<typename Type, typename CompareType, typename CollectionType = std::vector<Type>>
    CollectionType
    getSorted(const CollectionType& ts, const CompareType& compare) noexcept
    {
        auto sorted = ts;

        std::sort(sorted.begin(), sorted.end(), compare);

        return sorted;
    }
} // namespace nd::src::tools
