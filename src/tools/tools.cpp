#include "tools.hpp"

namespace nd::src::tools
{
    f64
    getDt(const f64 min) noexcept
    {
        using namespace std;
        using namespace std::chrono;

        static auto time = high_resolution_clock::now();

        const auto now = high_resolution_clock::now();
        const auto dt  = duration_cast<seconds>(now - time).count();

        time = now;

        return max(min, static_cast<f64>(dt));
    }
} // namespace nd::src::tools
