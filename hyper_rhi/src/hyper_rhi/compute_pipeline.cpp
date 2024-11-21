/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/compute_pipeline.hpp"

#include <hyper_core/assertion.hpp>

namespace he::rhi
{
    IComputePipeline::IComputePipeline(const ComputePipelineDescriptor &descriptor)
        : m_label(descriptor.label)
        , m_layout(descriptor.layout)
        , m_shader(descriptor.shader)
    {
        HE_ASSERT(!m_label.empty(), "Compute Pipeline wasn't given a label");

        HE_ASSERT(m_layout != nullptr);
        HE_ASSERT(m_shader != nullptr);
    }

    std::string_view IComputePipeline::label() const
    {
        return m_label;
    }

    const std::shared_ptr<IPipelineLayout> &IComputePipeline::layout() const
    {
        return m_layout;
    }

    const std::shared_ptr<IShaderModule> &IComputePipeline::shader() const
    {
        return m_shader;
    }
} // namespace he::rhi