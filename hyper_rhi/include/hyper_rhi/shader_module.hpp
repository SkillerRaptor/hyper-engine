/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <vector>

#include "hyper_rhi/shader_type.hpp"

namespace hyper_engine
{
    struct ShaderModuleDescriptor
    {
        std::string label;

        ShaderType type = ShaderType::None;
        std::string entry_name = "main";
        std::vector<uint8_t> bytes = {};
    };

    class ShaderModule
    {
    public:
        virtual ~ShaderModule() = default;

        virtual std::string_view label() const = 0;

        virtual ShaderType type() const = 0;
        virtual std::string_view entry_name() const = 0;
        virtual const std::vector<uint8_t> &bytes() const = 0;
    };
} // namespace hyper_engine
