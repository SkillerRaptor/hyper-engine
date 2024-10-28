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

    protected:
        explicit ShaderModule(std::string label);
    };

    using ShaderModuleHandle = std::shared_ptr<ShaderModule>;
} // namespace hyper_rhi
