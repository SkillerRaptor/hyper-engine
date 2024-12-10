/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_core/module.hpp"

namespace hyper_engine
{
    class HyperCore final : public Module
    {
    public:
        HyperCore();
        ~HyperCore() override;

        std::string_view module_name() const override;
    };
} // namespace hyper_engine