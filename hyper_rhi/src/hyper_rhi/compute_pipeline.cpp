/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/compute_pipeline.hpp"

namespace hyper_rhi
{
    ComputePipeline::ComputePipeline(const ComputePipelineDescriptor &descriptor)
        : Resource(descriptor.label)
        , m_layout(descriptor.layout)
        , m_shader(descriptor.shader)
    {
    }

    PipelineLayout &ComputePipeline::layout() const
    {
        return *m_layout;
    }

    ShaderModule &ComputePipeline::shader() const
    {
        return *m_shader;
    }
} // namespace hyper_rhi