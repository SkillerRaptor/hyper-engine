/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

namespace hyper_engine
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

        virtual std::string_view label() const = 0;

        virtual uint32_t push_constant_size() const = 0;
    };
} // namespace hyper_engine
