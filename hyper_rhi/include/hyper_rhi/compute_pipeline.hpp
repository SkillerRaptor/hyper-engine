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
    struct ComputePipelineDescriptor
    {
        std::string label;

        PipelineLayoutHandle layout = nullptr;
        ShaderModuleHandle shader = nullptr;
    };

    class ComputePipeline : public Resource
    {
    public:
        virtual ~ComputePipeline() = default;

    protected:
        explicit ComputePipeline(std::string label);
    };

    using ComputePipelineHandle = std::shared_ptr<ComputePipeline>;
} // namespace hyper_rhi
