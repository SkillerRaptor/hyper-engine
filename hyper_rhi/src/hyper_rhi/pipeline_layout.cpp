/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/pipeline_layout.hpp"

#include <hyper_core/assertion.hpp>

namespace he::rhi
{
    IPipelineLayout::IPipelineLayout(const PipelineLayoutDescriptor &descriptor)
        : m_label(descriptor.label)
        , m_push_constant_size(descriptor.push_constant_size)
    {
        HE_ASSERT(!m_label.empty(), "Pipeline Layout wasn't given a label");

        HE_ASSERT((m_push_constant_size % 4) == 0);
    }

    std::string_view IPipelineLayout::label() const
    {
        return m_label;
    }

    uint32_t IPipelineLayout::push_constant_size() const
    {
        return m_push_constant_size;
    }
} // namespace he::rhi