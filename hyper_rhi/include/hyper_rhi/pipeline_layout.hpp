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

    class IPipelineLayout
    {
    public:
        virtual ~IPipelineLayout() = default;

        std::string_view label() const;

        uint32_t push_constant_size() const;

    protected:
        explicit IPipelineLayout(const PipelineLayoutDescriptor &descriptor);

    protected:
        std::string m_label;

        uint32_t m_push_constant_size;
    };
} // namespace he::rhi
