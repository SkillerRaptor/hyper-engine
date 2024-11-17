/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/render_pass.hpp"

#include <hyper_core/assertion.hpp>

namespace he::rhi
{
    RenderPass::RenderPass(const RenderPassDescriptor &descriptor)
        : m_label(descriptor.label)
        , m_label_color(descriptor.label_color)
        , m_color_attachments(descriptor.color_attachments)
        , m_depth_stencil_attachment(descriptor.depth_stencil_attachment)
    {
        HE_ASSERT(!m_label.empty(), "Render Pass wasn't given a label");

        HE_ASSERT(!m_color_attachments.empty());
    }

    std::string_view RenderPass::label() const
    {
        return m_label;
    }

    LabelColor RenderPass::label_color() const
    {
        return m_label_color;
    }

    const std::vector<ColorAttachment> &RenderPass::color_attachments() const
    {
        return m_color_attachments;
    }

    DepthStencilAttachment RenderPass::depth_stencil_attachment() const
    {
        return m_depth_stencil_attachment;
    }
} // namespace he::rhi