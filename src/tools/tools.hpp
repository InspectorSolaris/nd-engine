#pragma once

#include "pch.hpp"

#include "types.hpp"
#include "scope.hpp"

#if defined(NDEBUG)
    #define ND_ASSERT_NOTHROW (true)
#else
    #define ND_ASSERT_NOTHROW (false)
#endif

#define ND_DECLARE_BUILDER_FIELD(field) decltype(Type::field) field

#define ND_DEFINE_BUILDER_SET(field)                        \
    Builder& set(const decltype(Type::field)& cfg) noexcept \
    {                                                       \
        field = cfg;                                        \
        return *this;                                       \
    }

#define ND_DEFINE_BUILDER_OPERATOR(field)                          \
    Builder& operator<<(const decltype(Type::field)& cfg) noexcept \
    {                                                              \
        return set(cfg);                                           \
    }

namespace nd::src::tools
{
    f64
    getDt(const f64 min) noexcept;
} // namespace nd::src::tools
