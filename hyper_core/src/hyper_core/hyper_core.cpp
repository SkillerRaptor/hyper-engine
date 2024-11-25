/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_core/hyper_core.hpp"

#include "hyper_core/assertion.hpp"
#include "hyper_core/global_environment.hpp"
#include "hyper_core/logger.hpp"

namespace hyper_engine
{
    HyperCore::HyperCore()
    {
        g_environment.logger = new Logger();
        HE_ASSERT(g_environment.logger);

        HE_INFO("Initialized HyperCore module");
    }

    HyperCore::~HyperCore()
    {
        HE_INFO("Shutdown HyperCore module");

        // NOTE: This is a special case, where the deleting doesn't happen before the final message, because it is the logger itself
        delete g_environment.logger;
    }

    std::string_view HyperCore::module_name() const
    {
        return "HyperCore";
    }
} // namespace hyper_engine