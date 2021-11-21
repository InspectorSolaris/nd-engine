#include "state.hpp"
#include "tools.hpp"

namespace nd::src::graphics::glfw
{
    bool State::s_initialized {};

    State::State()
    {
        ND_ASSERT(s_initialized);

        s_initialized = true;

        glfwInit();
    }

    State::~State()
    {
        s_initialized = false;

        glfwTerminate();
    }

    State
    getState()
    {
        return State();
    }
} // namespace nd::src::graphics::glfw
