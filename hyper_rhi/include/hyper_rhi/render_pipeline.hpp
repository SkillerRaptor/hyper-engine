/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include <hyper_core/bits.hpp>

#include "hyper_rhi/compare_operation.hpp"
#include "hyper_rhi/format.hpp"

namespace hyper_engine
{
    class IPipelineLayout;
    class IShaderModule;

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

        std::shared_ptr<IPipelineLayout> layout = nullptr;
        std::shared_ptr<IShaderModule> vertex_shader = nullptr;
        std::shared_ptr<IShaderModule> fragment_shader = nullptr;
        std::vector<ColorAttachmentState> color_attachment_states = {};
        PrimitiveState primitive_state = {};
        DepthStencilState depth_stencil_state = {};
    };

    class IRenderPipeline
    {
    public:
        virtual ~IRenderPipeline() = default;

        virtual std::string_view label() const = 0;

        virtual const std::shared_ptr<IPipelineLayout> &layout() const = 0;
        virtual const std::shared_ptr<IShaderModule> &vertex_shader() const = 0;
        virtual const std::shared_ptr<IShaderModule> &fragment_shader() const = 0;
        virtual const std::vector<ColorAttachmentState> &color_attachment_states() const = 0;
        virtual PrimitiveState primitive_state() const = 0;
        virtual DepthStencilState depth_stencil_state() const = 0;
    };
} // namespace hyper_engine
