/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_ecs/hyper_ecs.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_core/global_environment.hpp>
#include <hyper_core/logger.hpp>

namespace hyper_engine
{
    HyperEcs::HyperEcs()
    {
        HE_INFO("Initialized HyperEcs module");
    }

    HyperEcs::~HyperEcs()
    {
        HE_INFO("Shutdown HyperEcs module");
    }

    std::string_view HyperEcs::module_name() const
    {
        return "HyperEcs";
    }
} // namespace hyper_engine