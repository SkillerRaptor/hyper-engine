/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

#include "hyper_rhi/resource.hpp"

namespace hyper_rhi
{
    struct PipelineLayoutDescriptor
    {
        std::string label;

        uint32_t push_constant_size = 0;
    };

    class PipelineLayout : public Resource
    {
    public:
        virtual ~PipelineLayout() = default;

        [[nodiscard]] uint32_t push_constant_size() const;

    protected:
        explicit PipelineLayout(const PipelineLayoutDescriptor &descriptor);

    protected:
        uint32_t m_push_constant_size;
    };
} // namespace hyper_rhi
