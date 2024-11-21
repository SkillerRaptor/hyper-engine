/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/render_pass.hpp"

#include <hyper_core/assertion.hpp>

namespace he::rhi
{
    IRenderPass::IRenderPass(const RenderPassDescriptor &descriptor)
        : m_label(descriptor.label)
        , m_label_color(descriptor.label_color)
        , m_color_attachments(descriptor.color_attachments)
        , m_depth_stencil_attachment(descriptor.depth_stencil_attachment)
    {
        HE_ASSERT(!m_label.empty(), "Render Pass wasn't given a label");

        HE_ASSERT(!m_color_attachments.empty());
    }

    std::string_view IRenderPass::label() const
    {
        return m_label;
    }

    LabelColor IRenderPass::label_color() const
    {
        return m_label_color;
    }

    const std::vector<ColorAttachment> &IRenderPass::color_attachments() const
    {
        return m_color_attachments;
    }

    DepthStencilAttachment IRenderPass::depth_stencil_attachment() const
    {
        return m_depth_stencil_attachment;
    }
} // namespace he::rhi