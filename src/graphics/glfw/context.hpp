#pragma once

#include "pch.hpp"
#include "window.hpp"

namespace nd::src::graphics::glfw
{
    class Context
    {
    public:
        using Extension  = std::string;
        using Extensions = std::vector<Extension>;

        Context() noexcept;

        Context(const Context &context) = delete;
        Context(Context &&context)      = delete;

        Context &
        operator=(const Context &context) = delete;
        Context &
        operator=(Context &&context) = delete;

        ~Context();

        constexpr Window &
        getWindow() noexcept;

    private:
        Window window_;
    };

    constexpr Window &
    Context::getWindow() noexcept
    {
        return window_;
    }

    Context::Extensions
    getRequiredExtensions() noexcept;
}    // namespace nd::src::graphics::glfw
