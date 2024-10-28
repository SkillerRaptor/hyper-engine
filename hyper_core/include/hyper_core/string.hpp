/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

namespace hyper_core::string
{
    [[nodiscard]] std::wstring to_wstring(const std::string &string);
} // namespace hyper_core::string