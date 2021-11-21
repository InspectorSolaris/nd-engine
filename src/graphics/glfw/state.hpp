#pragma once

#include "pch.hpp"

namespace nd::src::graphics::glfw
{
    class State final
    {
    public:
        State();

        State(const State& state) = delete;
        State(State&& state)      = delete;

        State&
        operator=(const State& state) = delete;
        State&
        operator=(State&& state) = delete;

        ~State();

    private:
        static bool s_initialized;
    };

    State
    getState();
} // namespace nd::src::graphics::glfw
