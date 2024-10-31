/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

#include "hyper_rhi/pipeline_layout.hpp"
#include "hyper_rhi/resource.hpp"
#include "hyper_rhi/shader_module.hpp"

namespace hyper_rhi
{
    enum class Face
    {
        None,
        Front,
        Back
    };

    struct PrimitiveState
    {
        Face cull_mode;
    };

    enum class CompareOperation
    {
        Undefined,
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always,
    };

    struct DepthStencilState
    {
        bool depth_enabled = false;
        CompareOperation compare_operation = CompareOperation::Undefined;
    };

    struct GraphicsPipelineDescriptor
    {
        std::string label;

        std::shared_ptr<PipelineLayout> layout = nullptr;
        std::shared_ptr<ShaderModule> vertex_shader = nullptr;
        std::shared_ptr<ShaderModule> fragment_shader = nullptr;
        PrimitiveState primitive_state;
        DepthStencilState depth_stencil_state;
    };

    class GraphicsPipeline : public Resource
    {
    public:
        virtual ~GraphicsPipeline() = default;

        [[nodiscard]] PipelineLayout &layout() const;
        [[nodiscard]] ShaderModule &vertex_shader() const;
        [[nodiscard]] ShaderModule &fragment_shader() const;
        [[nodiscard]] PrimitiveState primitive_state() const;
        [[nodiscard]] DepthStencilState depth_stencil_state() const;

    protected:
        explicit GraphicsPipeline(const GraphicsPipelineDescriptor &descriptor);

    protected:
        std::shared_ptr<PipelineLayout> m_layout;
        std::shared_ptr<ShaderModule> m_vertex_shader;
        std::shared_ptr<ShaderModule> m_fragment_shader;
        PrimitiveState m_primitive_state;
        DepthStencilState m_depth_stencil_state;
    };
} // namespace hyper_rhi
