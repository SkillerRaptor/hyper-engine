/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

#include <hyper_core/nonnull_ref_ptr.hpp>
#include <hyper_core/ref_counted.hpp>

#include "hyper_rhi/forward.hpp"

namespace hyper_engine
{
    struct ComputePipelineDescriptor
    {
        std::string label;

        NonnullRefPtr<PipelineLayout> layout;
        NonnullRefPtr<ShaderModule> shader;
    };

    class ComputePipeline : public RefCounted<ComputePipeline>
    {
    public:
        virtual ~ComputePipeline() = default;

        std::string_view label() const;
        NonnullRefPtr<PipelineLayout> layout() const;
        NonnullRefPtr<ShaderModule> shader() const;

    protected:
        explicit ComputePipeline(const ComputePipelineDescriptor &descriptor);

    protected:
        std::string m_label;
        NonnullRefPtr<PipelineLayout> m_layout;
        NonnullRefPtr<ShaderModule> m_shader;
    };
} // namespace hyper_engine
