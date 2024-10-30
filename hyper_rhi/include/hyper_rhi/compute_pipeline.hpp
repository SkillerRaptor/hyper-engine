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

        std::shared_ptr<PipelineLayout> layout = nullptr;
        std::shared_ptr<ShaderModule> shader = nullptr;
    };

    class ComputePipeline : public Resource
    {
    public:
        virtual ~ComputePipeline() = default;

        [[nodiscard]] PipelineLayout &layout() const;
        [[nodiscard]] ShaderModule &shader() const;

    protected:
        explicit ComputePipeline(const ComputePipelineDescriptor &descriptor);

    protected:
        std::shared_ptr<PipelineLayout> m_layout;
        std::shared_ptr<ShaderModule> m_shader;
    };
} // namespace hyper_rhi
