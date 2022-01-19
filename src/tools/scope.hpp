#pragma once

#include "pch.hpp"

namespace nd::src::tools
{
    class Scope final
    {
    public:
        using Event = std::function<void(const std::shared_ptr<spdlog::logger>& logPtr, const std::string_view name, const int depth)>;

        Scope(const std::string_view name, const Event& onBegin, const Event& onEnd) noexcept;

        ~Scope();

        static void
        set(const std::shared_ptr<spdlog::logger> logPtr) noexcept
        {
            assert(s_logPtr == nullptr);

            s_logPtr = logPtr;
        }

    private:
        static std::shared_ptr<spdlog::logger> s_logPtr;
        static int                             s_depth;

        const Event&           onEnd_ {};
        const std::string_view name_ {};
    };

    void
    onScopeBegin(const std::shared_ptr<spdlog::logger>& logPtr, const std::string_view name, const int depth) noexcept;

    void
    onScopeEnd(const std::shared_ptr<spdlog::logger>& logPtr, const std::string_view name, const int depth) noexcept;
} // namespace nd::src::tools
