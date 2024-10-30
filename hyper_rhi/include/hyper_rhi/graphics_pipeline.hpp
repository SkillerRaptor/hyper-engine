/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <optional>
#include <string>

#include "hyper_rhi/pipeline_layout.hpp"
#include "hyper_rhi/resource.hpp"
#include "hyper_rhi/shader_module.hpp"

namespace hyper_rhi
{
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

    struct DepthState
    {
        bool depth_enabled = false;
        CompareOperation compare_operation = CompareOperation::Undefined;
    };

    struct GraphicsPipelineDescriptor
    {
        std::string label;

        PipelineLayoutHandle layout = nullptr;
        ShaderModuleHandle vertex_shader = nullptr;
        ShaderModuleHandle fragment_shader = nullptr;
        DepthState depth_state;
    };

    class GraphicsPipeline : public Resource
    {
    public:
        virtual ~GraphicsPipeline() = default;

        [[nodiscard]] PipelineLayoutHandle layout() const;

    protected:
        explicit GraphicsPipeline(const GraphicsPipelineDescriptor &descriptor);

    protected:
        PipelineLayoutHandle m_layout;
    };

    using GraphicsPipelineHandle = std::shared_ptr<GraphicsPipeline>;
} // namespace hyper_rhi
