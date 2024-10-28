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
    struct GraphicsPipelineDescriptor
    {
        std::string label;

        PipelineLayoutHandle layout = nullptr;
        ShaderModuleHandle vertex_shader = nullptr;
        ShaderModuleHandle fragment_shader = nullptr;
    };

    class GraphicsPipeline : public Resource
    {
    public:
        virtual ~GraphicsPipeline() = default;

    protected:
        explicit GraphicsPipeline(std::string label);
    };

    using GraphicsPipelineHandle = std::shared_ptr<GraphicsPipeline>;
} // namespace hyper_rhi
