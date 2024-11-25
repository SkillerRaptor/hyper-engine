/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/module.hpp>

namespace hyper_engine
{
    class HyperPlatform final : public IModule
    {
    public:
        HyperPlatform();
        ~HyperPlatform() override;

        std::string_view module_name() const override;
    };
} // namespace hyper_engine