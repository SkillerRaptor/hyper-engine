/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "hyper_rhi/resource.hpp"

namespace hyper_rhi
{
    enum class ShaderType
    {
        None,
        Compute,
        Fragment,
        Vertex
    };

    struct ShaderModuleDescriptor
    {
        std::string label;

        ShaderType type = ShaderType::None;
        std::string entry_name = "main";
        std::vector<uint8_t> bytes = {};
    };

    class ShaderModule : public Resource
    {
    public:
        virtual ~ShaderModule() = default;

        [[nodiscard]] ShaderType type() const;
        [[nodiscard]] std::string_view entry_name() const;
        [[nodiscard]] const std::vector<uint8_t> &bytes() const;

    protected:
        explicit ShaderModule(const ShaderModuleDescriptor &descriptor);

    protected:
        ShaderType m_type;
        std::string m_entry_name;
        std::vector<uint8_t> m_bytes;
    };
} // namespace hyper_rhi
