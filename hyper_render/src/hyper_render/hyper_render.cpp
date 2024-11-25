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
    HyperRender::HyperRender(const HyperRenderDescriptor &descriptor)
    {
        g_environment.renderer = new Renderer(
            {
                .graphics_api = descriptor.graphics_api,
                .debug_validation_enabled = descriptor.debug_validation_enabled,
                .debug_label_enabled = descriptor.debug_label_enabled,
                .debug_marker_enabled = descriptor.debug_marker_enabled,
            });
        HE_ASSERT(g_environment.renderer);

        HE_INFO("Initialized HyperRender module");
    }

    HyperRender::~HyperRender()
    {
        delete g_environment.renderer;

        HE_INFO("Shutdown HyperRender module");
    }

    std::string_view HyperRender::module_name() const
    {
        return "HyperRender";
    }
} // namespace hyper_engine