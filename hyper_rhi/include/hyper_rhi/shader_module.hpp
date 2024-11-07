/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <vector>

#include <hyper_core/assertion.hpp>
#include <hyper_core/prerequisites.hpp>

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

        [[nodiscard]] HE_FORCE_INLINE ShaderType type() const
        {
            return m_type;
        }

        [[nodiscard]] HE_FORCE_INLINE std::string_view entry_name() const
        {
            return m_entry_name;
        }

        [[nodiscard]] HE_FORCE_INLINE const std::vector<uint8_t> &bytes() const
        {
            return m_bytes;
        }

    protected:
        explicit ShaderModule(const ShaderModuleDescriptor &descriptor)
            : Resource(descriptor.label)
            , m_type(descriptor.type)
            , m_entry_name(descriptor.entry_name)
            , m_bytes(descriptor.bytes)
        {
            HE_ASSERT(m_type != ShaderType::None);
            HE_ASSERT(!m_entry_name.empty());
            HE_ASSERT(!m_bytes.empty());
        }

    protected:
        ShaderType m_type;
        std::string m_entry_name;
        std::vector<uint8_t> m_bytes;
    };
} // namespace hyper_rhi
