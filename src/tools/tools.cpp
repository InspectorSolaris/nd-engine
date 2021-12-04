#include "tools.hpp"

namespace nd::src::tools
{
    std::runtime_error
    getRuntimeError(const std::string_view file,
                    const std::string_view function,
                    const std::string_view condition,
                    const size_t           line) noexcept
    {
        return std::runtime_error(fmt::format("{}:{} ({}) {}", file, line, function, condition));
    }
} // namespace nd::src::tools
