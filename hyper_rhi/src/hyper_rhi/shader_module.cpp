/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/shader_module.hpp"

#include <hyper_core/assertion.hpp>

namespace he::rhi
{
    ShaderModule::ShaderModule(const ShaderModuleDescriptor &descriptor)
        : m_label(descriptor.label)
        , m_type(descriptor.type)
        , m_entry_name(descriptor.entry_name)
        , m_bytes(descriptor.bytes)
    {
        HE_ASSERT(!m_label.empty(), "Shader Module wasn't given a label");

        HE_ASSERT(m_type != ShaderType::None);
        HE_ASSERT(!m_entry_name.empty());
        HE_ASSERT(!m_bytes.empty());
    }

    std::string_view ShaderModule::label() const
    {
        return m_label;
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
} // namespace he::rhi