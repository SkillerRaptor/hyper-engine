/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string_view>
#include <vector>

namespace he::core::fs
{
    std::vector<uint8_t> read_file(std::string_view file_path);
} // namespace he::core::fs