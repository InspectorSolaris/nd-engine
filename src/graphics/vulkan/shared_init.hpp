#pragma once

#include "pch.hpp"
#include "tools.hpp"

#include "objects.hpp"
#include "objects_cfg.hpp"

#define ND_VK_ALLOCATION_CALLBACKS (nullptr)

#define ND_VK_ASSERT_EXEC_NOTHROW (ND_ASSERT_NOTHROW)
#define ND_VK_ASSERT_EXEC(result) \
    if(result != VK_SUCCESS)      \
    {                             \
        ND_ASSERT_STATIC();       \
    }

namespace nd::src::graphics::vulkan
{
} // namespace nd::src::graphics::vulkan
