/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

#include <hyper_core/assertion.hpp>
#include <hyper_core/prerequisites.hpp>

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

        [[nodiscard]] HE_FORCE_INLINE PipelineLayout &layout() const
        {
            return *m_layout;
        }

        [[nodiscard]] HE_FORCE_INLINE ShaderModule &shader() const
        {
            return *m_shader;
        }

    protected:
        explicit ComputePipeline(const ComputePipelineDescriptor &descriptor)
            : Resource(descriptor.label)
            , m_layout(descriptor.layout)
            , m_shader(descriptor.shader)
        {
            HE_ASSERT(m_layout != nullptr);
            HE_ASSERT(m_shader != nullptr);
        }

    protected:
        std::shared_ptr<PipelineLayout> m_layout;
        std::shared_ptr<ShaderModule> m_shader;
    };
} // namespace hyper_rhi
