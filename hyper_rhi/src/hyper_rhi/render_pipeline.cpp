/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/render_pipeline.hpp"

#include <hyper_core/assertion.hpp>

namespace he::rhi
{
    RenderPipeline::RenderPipeline(const RenderPipelineDescriptor &descriptor)
        : m_label(descriptor.label)
        , m_layout(descriptor.layout)
        , m_vertex_shader(descriptor.vertex_shader)
        , m_fragment_shader(descriptor.fragment_shader)
        , m_color_attachment_states(descriptor.color_attachment_states)
        , m_primitive_state(descriptor.primitive_state)
        , m_depth_stencil_state(descriptor.depth_stencil_state)
    {
        HE_ASSERT(!m_label.empty(), "Render Pipeline wasn't given a label");

        HE_ASSERT(m_layout != nullptr);
        HE_ASSERT(m_vertex_shader != nullptr);
        HE_ASSERT(m_fragment_shader != nullptr);
        HE_ASSERT(!m_color_attachment_states.empty());

        for (const ColorAttachmentState &color_attachment_state : m_color_attachment_states)
        {
            HE_ASSERT(color_attachment_state.format != Format::Unknown);
        }

        if (m_depth_stencil_state.depth_test_enable)
        {
            HE_ASSERT(m_depth_stencil_state.depth_format != Format::Unknown);
        }
    }

    std::string_view RenderPipeline::label() const
    {
        return m_label;
    }

    const std::shared_ptr<PipelineLayout> &RenderPipeline::layout() const
    {
        return m_layout;
    }

    const std::shared_ptr<ShaderModule> &RenderPipeline::vertex_shader() const
    {
        return m_vertex_shader;
    }

    const std::shared_ptr<ShaderModule> &RenderPipeline::fragment_shader() const
    {
        return m_fragment_shader;
    }

    const std::vector<ColorAttachmentState> &RenderPipeline::color_attachment_states() const
    {
        return m_color_attachment_states;
    }

    PrimitiveState RenderPipeline::primitive_state() const
    {
        return m_primitive_state;
    }

    DepthStencilState RenderPipeline::depth_stencil_state() const
    {
        return m_depth_stencil_state;
    }
} // namespace he::rhi