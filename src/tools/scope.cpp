#include "scope.hpp"

namespace nd::src::tools
{
    Scope::LogPtr Scope::s_logPtr {};
    Scope::Depth  Scope::s_depth {};

    Scope::Scope(const Name name, OnBegin *onBegin, OnEnd *onEnd) noexcept : name_(name), onEnd_(onEnd)
    {
        onBegin(s_logPtr, name_, ++s_depth);
    }

    Scope::~Scope()
    {
        onEnd_(s_logPtr, name_, s_depth--);
    }

    void
    onScopeBegin(const Scope::LogPtr logPtr, const Scope::Name name, const Scope::Depth depth) noexcept
    {
        logPtr->trace("[{}] {}", depth, name);
    }

    void
    onScopeEnd(const Scope::LogPtr logPtr, const Scope::Name name, const Scope::Depth depth) noexcept
    {
        logPtr->trace("[{}] {}", depth, name);
    }
}    // namespace nd::src::tools
