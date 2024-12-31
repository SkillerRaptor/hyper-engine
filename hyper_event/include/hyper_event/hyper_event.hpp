/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/module.hpp>

namespace hyper_engine
{
    class HyperEvent final : public Module
    {
    public:
        HyperEvent();
        ~HyperEvent() override;

        std::string_view module_name() const override;
    };
} // namespace hyper_engine