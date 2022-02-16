#pragma once

#include "pch.hpp"

#include "tools.hpp"

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

#define ND_USE_SCOPE_LEVEL 0

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
    #define ND_ASSERT_STATIC(code)
#else
    #define ND_ASSERT_EXEC(code)              \
        if(!(code))                           \
        {                                     \
            throw ND_GET_RUNTIME_ERROR(code); \
        }
    #define ND_ASSERT(code)    ND_ASSERT_EXEC(code)
    #define ND_ASSERT_STATIC() ND_ASSERT_EXEC(false)
#endif

namespace nd::src::tools
{
    const char* const logMainName  = "main";
    const char* const logScopeName = "scope";

    std::runtime_error
    getRuntimeError(const str_v file, const str_v function, const str_v reason, const u64 line) noexcept;

    bool
    isContainsAll(const u64, const u64) noexcept;

    bool
    isContainsAny(const u64, const u64) noexcept;

    u64
    getNextBit(const u64) noexcept;

    u8
    getNextBitIndex(const u64) noexcept;

    u8
    getBitIndex(const u64) noexcept;

    vec<str>
    getMerged(const vec<str>&, const vec<str>&) noexcept;

    vec<const char*>
    getRawStrings(const vec<str>&) noexcept;

    template<typename Type, typename FilterType>
    vec<Type>
    getFiltered(const span<const Type> collection, const FilterType&& filter) noexcept
    {
        auto filtered = vec<Type> {};

        for(u64 index = 0; index < collection.size(); ++index)
        {
            const auto& t = collection[index];

            if(filter(t, index))
            {
                filtered.push_back(t);
            }
        }

        return filtered;
    }

    template<typename Type, typename MapType>
    vec<Type>
    getMappedFlat(const u64 size, const MapType&& map) noexcept
    {
        auto mapped = vec<Type> {};

        mapped.reserve(size);

        for(u64 index = 0; index < size; ++index)
        {
            auto mappedItems = map(index);

            using std::begin;
            using std::end;
            using std::make_move_iterator;

            mapped.insert(end(mapped), make_move_iterator(begin(mappedItems)), make_move_iterator(end(mappedItems)));
        }

        return mapped;
    }

    template<typename Type, typename TypeResult, typename MapType>
    vec<TypeResult>
    getMappedFlat(const span<const Type> collection, const MapType&& map) noexcept
    {
        auto mapped = vec<TypeResult> {};

        mapped.reserve(collection.size());

        for(u64 index = 0; index < collection.size(); ++index)
        {
            auto mappedItems = map(collection[index], index);

            using std::begin;
            using std::end;
            using std::make_move_iterator;

            mapped.insert(end(mapped), make_move_iterator(begin(mappedItems)), make_move_iterator(end(mappedItems)));
        }

        return mapped;
    }

    template<typename Type, typename MapType>
    vec<Type>
    getMapped(const u64 size, const MapType&& map) noexcept
    {
        auto mapped = vec<Type> {};

        mapped.reserve(size);

        for(u64 index = 0; index < size; ++index)
        {
            mapped.push_back(map(index));
        }

        return mapped;
    }

    template<typename Type, typename TypeResult, typename MapType>
    vec<TypeResult>
    getMapped(const span<const Type> collection, const MapType&& map) noexcept
    {
        auto mapped = vec<TypeResult> {};

        mapped.reserve(collection.size());

        for(u64 index = 0; index < collection.size(); ++index)
        {
            mapped.push_back(map(collection[index], index));
        }

        return mapped;
    }

    template<typename Type, typename CompareType>
    vec<Type>
    getSorted(const vec<Type>& ts, const CompareType&& compare) noexcept
    {
        auto sorted = ts;

        std::sort(sorted.begin(), sorted.end(), compare);

        return sorted;
    }
} // namespace nd::src::tools
