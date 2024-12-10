/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_platform/hyper_platform.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_core/global_environment.hpp>
#include <hyper_core/logger.hpp>

#include "hyper_platform/input.hpp"
#include "hyper_platform/window.hpp"

namespace hyper_engine
{
    HyperPlatform::HyperPlatform()
    {
        g_env.input = new Input();
        HE_ASSERT(g_env.input);

        g_env.window = new Window({
            .title = "HyperEngine",
            .width = 1280,
            .height = 720,
        });
        HE_ASSERT(g_env.window);

        HE_INFO("Initialized HyperPlatform module");
    }

    HyperPlatform::~HyperPlatform()
    {
        delete g_env.window;
        delete g_env.input;

        HE_INFO("Shutdown HyperPlatform module");
    }

    std::string_view HyperPlatform::module_name() const
    {
        return "HyperPlatform";
    }
} // namespace hyper_engine