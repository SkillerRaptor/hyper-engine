/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/module.hpp>
#include <hyper_rhi/graphics_device.hpp>

namespace hyper_engine
{
    struct HyperRenderDescriptor
    {
        GraphicsApi graphics_api;
        bool debug_validation_enabled;
        bool debug_label_enabled;
        bool debug_marker_enabled;
    };

    class HyperRender final : public IModule
    {
    public:
        explicit HyperRender(const HyperRenderDescriptor &descriptor);
        ~HyperRender() override;

        std::string_view module_name() const override;
    };
} // namespace hyper_engine