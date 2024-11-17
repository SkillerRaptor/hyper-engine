/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/compute_pass.hpp"

#include <hyper_core/assertion.hpp>

namespace he::rhi
{
    ComputePass::ComputePass(const ComputePassDescriptor &descriptor)
        : m_label(descriptor.label)
        , m_label_color(descriptor.label_color)
    {
        HE_ASSERT(!m_label.empty(), "Compute Pass wasn't given a label");
    }

    std::string_view ComputePass::label() const
    {
        return m_label;
    }

    LabelColor ComputePass::label_color() const
    {
        return m_label_color;
    }
} // namespace he::rhi