#pragma once

#include "pch.hpp"

namespace nd::src::tools
{
    class Scope final
    {
    public:
        using Event = std::function<void(const std::shared_ptr<spdlog::logger>& logPtr, const std::string name, const uint64_t depth)>;

        Scope(const std::string& name, const Event& onBegin, const Event& onEnd) noexcept;

        ~Scope();

        static void
        set(const std::shared_ptr<spdlog::logger> logPtr) noexcept
        {
            assert(s_logPtr == nullptr);

            s_logPtr = logPtr;
        }

    private:
        static std::shared_ptr<spdlog::logger> s_logPtr;
        static uint64_t                        s_depth;

        const Event&      onEnd_ {};
        const std::string name_ {};
    };

    void
    onScopeBegin(const std::shared_ptr<spdlog::logger>& logPtr, const std::string& name, const uint64_t depth) noexcept;

    void
    onScopeEnd(const std::shared_ptr<spdlog::logger>& logPtr, const std::string& name, const uint64_t depth) noexcept;
} // namespace nd::src::tools
