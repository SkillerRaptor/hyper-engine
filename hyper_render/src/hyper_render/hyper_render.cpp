/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/hyper_render.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_core/global_environment.hpp>
#include <hyper_core/logger.hpp>

#include "hyper_render/renderer.hpp"

namespace hyper_engine
{
    HyperRender::HyperRender()
    {
        g_env.renderer = new Renderer();
        HE_ASSERT(g_env.renderer);

        HE_INFO("Initialized HyperRender module");
    }

    HyperRender::~HyperRender()
    {
        delete g_env.renderer;

        HE_INFO("Shutdown HyperRender module");
    }

    std::string_view HyperRender::module_name() const
    {
        return "HyperRender";
    }
} // namespace hyper_engine