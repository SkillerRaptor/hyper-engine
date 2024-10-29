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
    {
    }
} // namespace hyper_rhi