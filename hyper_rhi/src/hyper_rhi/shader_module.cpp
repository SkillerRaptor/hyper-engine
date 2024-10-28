/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/shader_module.hpp"

#include <utility>

namespace hyper_rhi
{
    ShaderModule::ShaderModule(std::string label)
        : Resource(std::move(label))
    {
    }
} // namespace hyper_rhi