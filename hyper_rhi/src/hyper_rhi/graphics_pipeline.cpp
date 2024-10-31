/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/graphics_pipeline.hpp"

namespace hyper_rhi
{
    GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineDescriptor &descriptor)
        : Resource(descriptor.label)
        , m_layout(descriptor.layout)
        , m_vertex_shader(descriptor.vertex_shader)
        , m_fragment_shader(descriptor.fragment_shader)
        , m_primitive_state(descriptor.primitive_state)
        , m_depth_stencil_state(descriptor.depth_stencil_state)
    {
    }

    PipelineLayout &GraphicsPipeline::layout() const
    {
        return *m_layout;
    }

    ShaderModule &GraphicsPipeline::vertex_shader() const
    {
        return *m_vertex_shader;
    }

    ShaderModule &GraphicsPipeline::fragment_shader() const
    {
        return *m_fragment_shader;
    }

    PrimitiveState GraphicsPipeline::primitive_state() const
    {
        return m_primitive_state;
    }

    DepthStencilState GraphicsPipeline::depth_stencil_state() const
    {
        return m_depth_stencil_state;
    }
} // namespace hyper_rhi