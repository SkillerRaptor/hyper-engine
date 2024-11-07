/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

#include <hyper_core/assertion.hpp>
#include <hyper_core/bitmask.hpp>
#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/pipeline_layout.hpp"
#include "hyper_rhi/resource.hpp"
#include "hyper_rhi/shader_module.hpp"
#include "hyper_rhi/texture.hpp"

namespace hyper_rhi
{
    enum class BlendFactor : uint8_t
    {
        Zero,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        ConstantColor,
        OneMinusConstantColor,
        ConstantAlpha,
        OneMinusConstantAlpha,
        SrcAlphaSaturate,
        Src1Color,
        OneMinusSrc1Color,
        Src1Alpha,
        OneMinusSrc1Alpha,
    };

    enum class BlendOperation : uint8_t
    {
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max,
    };

    enum class ColorWrites : uint8_t
    {
        R = 1 << 0,
        G = 1 << 1,
        B = 1 << 2,
        A = 1 << 3,
        Color = R | G | B,
        All = R | G | B | A,
    };

    HE_ENABLE_BITMASK_OPERATORS(ColorWrites);

    struct BlendState
    {
        bool blend_enable = false;
        BlendFactor src_blend_factor = BlendFactor::One;
        BlendFactor dst_blend_factor = BlendFactor::Zero;
        BlendOperation operation = BlendOperation::Add;
        BlendFactor alpha_src_blend_factor = BlendFactor::One;
        BlendFactor alpha_dst_blend_factor = BlendFactor::Zero;
        BlendOperation alpha_operation = BlendOperation::Add;
        ColorWrites color_writes = ColorWrites::All;
    };

    struct ColorAttachmentState
    {
        Format format = Format::Unknown;
        BlendState blend_state = {};
    };

    enum class PrimitiveTopology : uint8_t
    {
        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,
        TriangleFan,
    };

    enum class FrontFace : uint8_t
    {
        CounterClockwise,
        Clockwise,
    };

    enum class Face : uint8_t
    {
        None,
        Front,
        Back
    };

    enum class PolygonMode : uint8_t
    {
        Fill,
        Line,
        Point,
    };

    struct PrimitiveState
    {
        PrimitiveTopology topology = PrimitiveTopology::TriangleList;
        FrontFace front_face = FrontFace::CounterClockwise;
        Face cull_mode = Face::None;
        PolygonMode polygon_mode = PolygonMode::Fill;
    };

    enum class CompareOperation : uint8_t
    {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always,
    };

    struct DepthBiasState
    {
        bool depth_bias_enable = false;
        float constant = 0.0f;
        float clamp = 0.0f;
        float slope = 0.0f;
    };

    struct DepthStencilState
    {
        bool depth_test_enable = false;
        bool depth_write_enable = false;
        Format depth_format = Format::Unknown;
        CompareOperation depth_compare_operation = CompareOperation::Less;
        DepthBiasState depth_bias_state = {};
    };

    struct RenderPipelineDescriptor
    {
        std::string label;

        std::shared_ptr<PipelineLayout> layout = nullptr;
        std::shared_ptr<ShaderModule> vertex_shader = nullptr;
        std::shared_ptr<ShaderModule> fragment_shader = nullptr;
        std::vector<ColorAttachmentState> color_attachment_states = {};
        PrimitiveState primitive_state = {};
        DepthStencilState depth_stencil_state = {};
    };

    class RenderPipeline : public Resource
    {
    public:
        virtual ~RenderPipeline() = default;

        [[nodiscard]] HE_FORCE_INLINE PipelineLayout &layout() const
        {
            return *m_layout;
        }

        [[nodiscard]] HE_FORCE_INLINE ShaderModule &vertex_shader() const
        {
            return *m_vertex_shader;
        }

        [[nodiscard]] HE_FORCE_INLINE ShaderModule &fragment_shader() const
        {
            return *m_fragment_shader;
        }

        [[nodiscard]] HE_FORCE_INLINE const std::vector<ColorAttachmentState> &color_attachment_states() const
        {
            return m_color_attachment_states;
        }

        [[nodiscard]] HE_FORCE_INLINE PrimitiveState primitive_state() const
        {
            return m_primitive_state;
        }

        [[nodiscard]] HE_FORCE_INLINE DepthStencilState depth_stencil_state() const
        {
            return m_depth_stencil_state;
        }

    protected:
        explicit RenderPipeline(const RenderPipelineDescriptor &descriptor)
            : Resource(descriptor.label)
            , m_layout(descriptor.layout)
            , m_vertex_shader(descriptor.vertex_shader)
            , m_fragment_shader(descriptor.fragment_shader)
            , m_color_attachment_states(descriptor.color_attachment_states)
            , m_primitive_state(descriptor.primitive_state)
            , m_depth_stencil_state(descriptor.depth_stencil_state)
        {
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

    protected:
        std::shared_ptr<PipelineLayout> m_layout;
        std::shared_ptr<ShaderModule> m_vertex_shader;
        std::shared_ptr<ShaderModule> m_fragment_shader;
        std::vector<ColorAttachmentState> m_color_attachment_states;
        PrimitiveState m_primitive_state;
        DepthStencilState m_depth_stencil_state;
    };
} // namespace hyper_rhi
