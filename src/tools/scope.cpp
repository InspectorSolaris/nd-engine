#include "scope.hpp"

namespace nd::src::tools
{
    shared<logger> Scope::s_logPtr {};
    u64            Scope::s_depth {};

    Scope::Scope(const str_v name, const Event& onStart, const Event& onEnd) noexcept
        : onEnd_(onEnd)
        , name_(name)
    {
        onStart(s_logPtr, name_, ++s_depth);
    }

    Scope::~Scope()
    {
        onEnd_(s_logPtr, name_, s_depth--);
    }

    void
    onScopeBegin(const shared<logger>& logPtr, const str_v name, const u64 depth) noexcept
    {
        logPtr->trace("[{}] {}", depth, name);
    }

    void
    onScopeEnd(const shared<logger>& logPtr, const str_v name, const u64 depth) noexcept
    {
        logPtr->trace("[{}] {}", depth, name);
    }
} // namespace nd::src::tools
