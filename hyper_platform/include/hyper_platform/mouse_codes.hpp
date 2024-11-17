/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>

namespace he::platform
{
    enum class MouseCode : uint8_t
    {
        ButtonLeft = 1,
        ButtonMiddle = 2,
        ButtonRight = 3,
        ButtonX1 = 4,
        ButtonX2 = 5,
    };
} // namespace he::platform
