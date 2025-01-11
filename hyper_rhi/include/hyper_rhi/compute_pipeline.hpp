/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

#include <hyper_core/ref_ptr.hpp>

#include "hyper_rhi/forward.hpp"

namespace hyper_engine
{
    struct ComputePipelineDescriptor
    {
        std::string label;

        RefPtr<PipelineLayout> layout;
        RefPtr<ShaderModule> shader;
    };

    class ComputePipeline
    {
    public:
        virtual ~ComputePipeline() = default;

        std::string_view label() const;
        RefPtr<PipelineLayout> layout() const;
        RefPtr<ShaderModule> shader() const;

    protected:
        explicit ComputePipeline(const ComputePipelineDescriptor &descriptor);

    protected:
        std::string m_label;
        RefPtr<PipelineLayout> m_layout;
        RefPtr<ShaderModule> m_shader;
    };
} // namespace hyper_engine
