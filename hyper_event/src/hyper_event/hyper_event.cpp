/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_event/hyper_event.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_core/global_environment.hpp>
#include <hyper_core/logger.hpp>

#include "hyper_event/event_bus.hpp"

namespace hyper_engine
{
    HyperEvent::HyperEvent()
    {
        g_env.event_bus = new EventBus();
        HE_ASSERT(g_env.event_bus);

        HE_INFO("Initialized HyperEvent module");
    }

    HyperEvent::~HyperEvent()
    {
        delete g_env.event_bus;

        HE_INFO("Shutdown HyperEvent module");
    }

    std::string_view HyperEvent::module_name() const
    {
        return "HyperEvent";
    }
} // namespace hyper_engine