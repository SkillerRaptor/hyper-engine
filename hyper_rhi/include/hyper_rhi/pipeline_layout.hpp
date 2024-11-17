/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

namespace he::rhi
{
    struct PipelineLayoutDescriptor
    {
        std::string label;

        uint32_t push_constant_size = 0;
    };

    class PipelineLayout
    {
    public:
        virtual ~PipelineLayout() = default;

        std::string_view label() const;

        uint32_t push_constant_size() const;

    protected:
        explicit PipelineLayout(const PipelineLayoutDescriptor &descriptor);

    protected:
        std::string m_label;

        uint32_t m_push_constant_size;
    };
} // namespace he::rhi
