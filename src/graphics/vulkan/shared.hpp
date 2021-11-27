#pragma once

#include "pch.hpp"

namespace nd::src::graphics::vulkan
{
    using Layer      = std::string;
    using Layers     = std::vector<Layer>;
    using Extension  = std::string;
    using Extensions = std::vector<Extension>;

    bool
    isSubmask(const uint32_t mask, const uint32_t submask) noexcept;

    uint32_t
    getNextBit(const uint32_t bits) noexcept;

    std::vector<std::string>
    getMerged(const std::vector<std::string>& data1, const std::vector<std::string>& data2) noexcept;

    std::vector<const char*>
    getCStrings(const std::vector<std::string>& data) noexcept;
} // namespace nd::src::graphics::vulkan
