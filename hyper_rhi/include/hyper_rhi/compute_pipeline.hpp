/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

namespace he::rhi
{
    class PipelineLayout;
    class ShaderModule;

    struct ComputePipelineDescriptor
    {
        std::string label;

        std::shared_ptr<PipelineLayout> layout = nullptr;
        std::shared_ptr<ShaderModule> shader = nullptr;
    };

    class ComputePipeline
    {
    public:
        virtual ~ComputePipeline() = default;

        std::string_view label() const;

        const std::shared_ptr<PipelineLayout> &layout() const;
        const std::shared_ptr<ShaderModule> &shader() const;

    protected:
        explicit ComputePipeline(const ComputePipelineDescriptor &descriptor);

    protected:
        std::string m_label;

        std::shared_ptr<PipelineLayout> m_layout;
        std::shared_ptr<ShaderModule> m_shader;
    };
} // namespace he::rhi
