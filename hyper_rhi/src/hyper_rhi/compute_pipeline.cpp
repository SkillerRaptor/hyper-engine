/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/compute_pipeline.hpp"

#include <hyper_core/assertion.hpp>

namespace he::rhi
{
    ComputePipeline::ComputePipeline(const ComputePipelineDescriptor &descriptor)
        : m_label(descriptor.label)
        , m_layout(descriptor.layout)
        , m_shader(descriptor.shader)
    {
        HE_ASSERT(!m_label.empty(), "Compute Pipeline wasn't given a label");

        HE_ASSERT(m_layout != nullptr);
        HE_ASSERT(m_shader != nullptr);
    }

    std::string_view ComputePipeline::label() const
    {
        return m_label;
    }

    const std::shared_ptr<PipelineLayout> &ComputePipeline::layout() const
    {
        return m_layout;
    }

    const std::shared_ptr<ShaderModule> &ComputePipeline::shader() const
    {
        return m_shader;
    }
} // namespace he::rhi