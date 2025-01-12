/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/module.hpp>

namespace hyper_engine
{
    class HyperEcs final : public Module
    {
    public:
        HyperEcs();
        ~HyperEcs() override;

        std::string_view module_name() const override;
    };
} // namespace hyper_engine