/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/render_pass.hpp"

namespace hyper_rhi
{
    RenderPass::RenderPass(const RenderPassDescriptor &descriptor)
        : Resource(descriptor.label)
        , m_label_color(descriptor.label_color)
        , m_color_attachment(descriptor.color_attachment)
        , m_depth_attachment(descriptor.depth_attachment)
    {
    }

    LabelColor RenderPass::label_color() const
    {
        return m_label_color;
    }

    ColorAttachment RenderPass::color_attachment() const
    {
        return m_color_attachment;
    }

    DepthAttachment RenderPass::depth_attachment() const
    {
        return m_depth_attachment;
    }
} // namespace hyper_rhi