#include "tools.hpp"

namespace nd::src::tools
{
    std::runtime_error
    getRuntimeError(const std::string_view file,
                    const std::string_view function,
                    const std::string_view condition,
                    const size_t           line) noexcept
    {
        const auto logMain = spdlog::get(logMainName);
        const auto message = fmt::format("File: {}. Function: {}. Condition: {}. Line: {}", file, function, condition, line);

        logMain->critical(message);

        return std::runtime_error("");
    }
} // namespace nd::src::tools
