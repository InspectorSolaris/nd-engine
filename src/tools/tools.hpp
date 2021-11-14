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

#define ND_USE_SCOPE_LEVEL 0

#if(ND_USE_SCOPE_LEVEL <= ND_USE_SCOPE)
    #define ND_SET_SCOPE() \
        const auto scope = nd::src::tools::Scope(ND_FUNCTION, nd::src::tools::onScopeBegin, nd::src::tools::onScopeEnd)
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

#define ND_GET_RUNTIME_ERROR(CONDITION) nd::src::tools::getRuntimeError(__FILE__, ND_FUNCTION, #CONDITION, __LINE__)

#if defined(NDEBUG)
    #define ND_ASSERT(CONDITION)
#else
    #define ND_ASSERT(CONDITION)                   \
        if(!(CONDITION))                           \
        {                                          \
            throw ND_GET_RUNTIME_ERROR(CONDITION); \
        }
#endif

namespace nd::src::tools
{
    const char* const logMainName  = "main";
    const char* const logScopeName = "scope";

    std::runtime_error
    getRuntimeError(const std::string_view file,
                    const std::string_view function,
                    const std::string_view condition,
                    const size_t           line) noexcept;
} // namespace nd::src::tools
