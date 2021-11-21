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

        using OnBegin = std::function<void(const LogPtr logPtr, const Name name, const Depth depth)>;
        using OnEnd   = std::function<void(const LogPtr logPtr, const Name name, const Depth depth)>;

        Scope(const Name name, const OnBegin& onBegin, const OnEnd& onEnd) noexcept;

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

        const Name   name_ {};
        const OnEnd& onEnd_ {};
    };

    void
    onScopeBegin(const Scope::LogPtr logPtr, const Scope::Name name, const Scope::Depth depth) noexcept;

    void
    onScopeEnd(const Scope::LogPtr logPtr, const Scope::Name name, const Scope::Depth depth) noexcept;
} // namespace nd::src::tools
