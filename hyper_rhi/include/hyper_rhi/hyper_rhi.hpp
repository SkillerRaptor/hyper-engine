/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/module.hpp>
#include <hyper_rhi/graphics_device.hpp>

namespace hyper_engine
{
    struct HyperRhiDescriptor
    {
        GraphicsApi graphics_api = GraphicsApi::Vulkan;
        bool debug_validation_enabled = false;
        bool debug_label_enabled = false;
        bool debug_marker_enabled = false;
    };

    class HyperRhi final : public Module
    {
    public:
        explicit HyperRhi(const HyperRhiDescriptor &descriptor);
        ~HyperRhi() override;

        std::string_view module_name() const override;
    };
} // namespace hyper_engine