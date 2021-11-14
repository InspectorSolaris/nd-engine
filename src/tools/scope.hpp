#pragma once

#include "pch.hpp"

namespace nd::src::tools
{
    class Scope final
    {
    public:
        using Log    = spdlog::logger;
        using LogPtr = std::shared_ptr<Log>;

        using Name  = std::string_view;
        using Depth = uint64_t;

        using OnBegin = void(const LogPtr logPtr, const Name name, const Depth depth);
        using OnEnd   = void(const LogPtr logPtr, const Name name, const Depth depth);

        Scope(const Name name, OnBegin *onBegin, OnEnd *onEnd) noexcept;

        ~Scope();

        static void
        set(LogPtr logPtr) noexcept
        {
            assert(s_logPtr == nullptr);

            s_logPtr = logPtr;
        }

    private:
        static LogPtr s_logPtr;
        static Depth  s_depth;

        Name   name_ {};
        OnEnd *onEnd_ {};
    };

    void
    onScopeBegin(const Scope::LogPtr logPtr, const Scope::Name name, const Scope::Depth depth) noexcept;

    void
    onScopeEnd(const Scope::LogPtr logPtr, const Scope::Name name, const Scope::Depth depth) noexcept;
}    // namespace nd::src::tools
