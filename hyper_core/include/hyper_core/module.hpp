/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string_view>

namespace hyper_engine
{
    struct GlobalEnvironment;

    class IModule
    {
    public:
        virtual ~IModule() = default;

        virtual std::string_view module_name() const = 0;
    };
} // namespace hyper_engine