/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string_view>

namespace hyper_engine
{
    class Module
    {
    public:
        virtual ~Module() = default;

        virtual std::string_view module_name() const = 0;
    };
} // namespace hyper_engine