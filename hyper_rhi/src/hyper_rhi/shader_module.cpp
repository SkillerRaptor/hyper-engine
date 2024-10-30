/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/shader_module.hpp"

namespace hyper_rhi
{
    ShaderModule::ShaderModule(const ShaderModuleDescriptor &descriptor)
        : Resource(descriptor.label)
        , m_type(descriptor.type)
        , m_entry_name(descriptor.entry_name)
        , m_bytes(descriptor.bytes)
    {
    }

    ShaderType ShaderModule::type() const
    {
        return m_type;
    }

    std::string_view ShaderModule::entry_name() const
    {
        return m_entry_name;
    }

    const std::vector<uint8_t> &ShaderModule::bytes() const
    {
        return m_bytes;
    }
} // namespace hyper_rhi