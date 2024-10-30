/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/pipeline_layout.hpp"

namespace hyper_rhi
{
    PipelineLayout::PipelineLayout(const PipelineLayoutDescriptor &descriptor)
        : Resource(descriptor.label)
        , m_push_constant_size(descriptor.push_constant_size)
    {
    }

    uint32_t PipelineLayout::push_constant_size() const
    {
        return m_push_constant_size;
    }
} // namespace hyper_rhi