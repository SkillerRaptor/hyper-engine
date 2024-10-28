/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

#include "hyper_rhi/resource.hpp"

namespace hyper_rhi
{
    struct PipelineLayoutDescriptor
    {
        std::string label;

        uint32_t push_constant_size;
    };

    class PipelineLayout : public Resource
    {
    public:
        virtual ~PipelineLayout() = default;

    protected:
        explicit PipelineLayout(std::string label);
    };

    using PipelineLayoutHandle = std::shared_ptr<PipelineLayout>;
} // namespace hyper_rhi
