#pragma once

#include "pch.hpp"

#include "types.hpp"

namespace nd::src::tools
{
    class Scope final
    {
    public:
        using Event = func<void(const shared<logger>&, const str_v, const u64)>;

        Scope(const str_v, const Event&, const Event&) noexcept;

        ~Scope();

        static void
        set(const shared<logger> logPtr) noexcept
        {
            assert(s_logPtr == nullptr);

            s_logPtr = logPtr;
        }

    private:
        static shared<logger> s_logPtr;
        static u64            s_depth;

        const Event& onEnd_ {};
        const str_v  name_ {};
    };

    void
    onScopeBegin(const shared<logger>&, const str_v, const u64) noexcept;

    void
    onScopeEnd(const shared<logger>&, const str_v, const u64) noexcept;
} // namespace nd::src::tools
