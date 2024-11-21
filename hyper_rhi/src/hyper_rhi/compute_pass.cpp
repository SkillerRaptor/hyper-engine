/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/compute_pass.hpp"

#include <hyper_core/assertion.hpp>

namespace hyper_engine
{
    IComputePass::IComputePass(const ComputePassDescriptor &descriptor)
        : m_label(descriptor.label)
        , m_label_color(descriptor.label_color)
    {
        HE_ASSERT(!m_label.empty(), "Compute Pass wasn't given a label");
    }

    std::string_view IComputePass::label() const
    {
        return m_label;
    }

    LabelColor IComputePass::label_color() const
    {
        return m_label_color;
    }
} // namespace hyper_engine