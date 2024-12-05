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

        virtual std::string_view label() const = 0;

        virtual const std::shared_ptr<IPipelineLayout> &layout() const = 0;
        virtual const std::shared_ptr<IShaderModule> &shader() const = 0;
    };
} // namespace hyper_engine
