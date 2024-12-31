/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/hyper_rhi.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_core/global_environment.hpp>
#include <hyper_core/logger.hpp>

#include "hyper_rhi/graphics_device.hpp"

namespace hyper_engine
{
    HyperRhi::HyperRhi(const HyperRhiDescriptor &descriptor)
    {
        g_env.graphics_device = GraphicsDevice::create({
            .graphics_api = descriptor.graphics_api,
            .debug_validation = descriptor.debug_validation_enabled,
            .debug_label = descriptor.debug_label_enabled,
            .debug_marker = descriptor.debug_marker_enabled,
        });
        HE_ASSERT(g_env.graphics_device);

        HE_INFO("Initialized HyperRhi module");
    }

    HyperRhi::~HyperRhi()
    {
        delete g_env.graphics_device;

        HE_INFO("Shutdown HyperRhi module");
    }

    std::string_view HyperRhi::module_name() const
    {
        return "HyperRhi";
    }
} // namespace hyper_engine