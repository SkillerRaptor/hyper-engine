/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>

namespace he::rhi
{
    enum class ShaderType : uint8_t
    {
        None,
        Compute,
        Fragment,
        Vertex
    };
} // namespace he::rhi