/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_core/hyper_core.hpp"

#include "hyper_core/assertion.hpp"
#include "hyper_core/global_environment.hpp"
#include "hyper_core/job_system.hpp"
#include "hyper_core/logger.hpp"

namespace hyper_engine
{
    HyperCore::HyperCore()
    {
        g_env.logger = new Logger();
        HE_ASSERT(g_env.logger);

        g_env.job_system = new JobSystem();
        HE_ASSERT(g_env.job_system);

        HE_INFO("Initialized HyperCore module");
    }

    HyperCore::~HyperCore()
    {
        delete g_env.job_system;

        HE_INFO("Shutdown HyperCore module");

        // NOTE: This is a special case, where the deleting doesn't happen before the final message, because it is the logger itself
        delete g_env.logger;
    }

    std::string_view HyperCore::module_name() const
    {
        return "HyperCore";
    }
} // namespace hyper_engine