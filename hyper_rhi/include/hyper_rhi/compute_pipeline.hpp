/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

namespace hyper_engine
{
    class PipelineLayout;
    class ShaderModule;

    struct ComputePipelineDescriptor
    {
        std::string label;

        std::shared_ptr<PipelineLayout> layout = nullptr;
        std::shared_ptr<ShaderModule> shader = nullptr;
    };

    class ComputePipeline
    {
    public:
        virtual ~ComputePipeline() = default;

        virtual std::string_view label() const = 0;

        virtual const std::shared_ptr<PipelineLayout> &layout() const = 0;
        virtual const std::shared_ptr<ShaderModule> &shader() const = 0;
    };
} // namespace hyper_engine
