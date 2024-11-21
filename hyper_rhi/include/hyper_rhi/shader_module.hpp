/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <vector>

#include "hyper_rhi/shader_type.hpp"

namespace he::rhi
{
    struct ShaderModuleDescriptor
    {
        std::string label;

        ShaderType type = ShaderType::None;
        std::string entry_name = "main";
        std::vector<uint8_t> bytes = {};
    };

    class IShaderModule
    {
    public:
        virtual ~IShaderModule() = default;

        std::string_view label() const;

        ShaderType type() const;
        std::string_view entry_name() const;
        const std::vector<uint8_t> &bytes() const;

    protected:
        explicit IShaderModule(const ShaderModuleDescriptor &descriptor);

    protected:
        std::string m_label;

        ShaderType m_type;
        std::string m_entry_name;
        std::vector<uint8_t> m_bytes;
    };
} // namespace he::rhi
