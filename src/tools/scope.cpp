#include "scope.hpp"

namespace nd::src::tools
{
    std::shared_ptr<spdlog::logger> Scope::s_logPtr {};
    uint64_t                        Scope::s_depth {};

    Scope::Scope(const std::string& name, const Event& onBegin, const Event& onEnd) noexcept
        : onEnd_(onEnd)
        , name_(name)
    {
        onBegin(s_logPtr, name_, ++s_depth);
    }

    Scope::~Scope()
    {
        onEnd_(s_logPtr, name_, s_depth--);
    }

    void
    onScopeBegin(const std::shared_ptr<spdlog::logger> logPtr, const std::string& name, const uint64_t depth) noexcept
    {
        logPtr->trace("[{}] {}", depth, name);
    }

    void
    onScopeEnd(const std::shared_ptr<spdlog::logger> logPtr, const std::string& name, const uint64_t depth) noexcept
    {
        logPtr->trace("[{}] {}", depth, name);
    }
} // namespace nd::src::tools
