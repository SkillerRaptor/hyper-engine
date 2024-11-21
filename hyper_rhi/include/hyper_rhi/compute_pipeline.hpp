/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

namespace hyper_engine
{
    class IPipelineLayout;
    class IShaderModule;

    struct ComputePipelineDescriptor
    {
        std::string label;

        std::shared_ptr<IPipelineLayout> layout = nullptr;
        std::shared_ptr<IShaderModule> shader = nullptr;
    };

    class IComputePipeline
    {
    public:
        virtual ~IComputePipeline() = default;

        std::string_view label() const;

        const std::shared_ptr<IPipelineLayout> &layout() const;
        const std::shared_ptr<IShaderModule> &shader() const;

    protected:
        explicit IComputePipeline(const ComputePipelineDescriptor &descriptor);

    protected:
        std::string m_label;

        std::shared_ptr<IPipelineLayout> m_layout;
        std::shared_ptr<IShaderModule> m_shader;
    };
} // namespace hyper_engine
