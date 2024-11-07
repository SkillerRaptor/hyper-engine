/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/prerequisites.hpp>

#include <string>

namespace hyper_rhi
{
    class Resource
    {
    public:
        [[nodiscard]] HE_FORCE_INLINE std::string_view label() const
        {
            return m_label;
        }

    protected:
        explicit Resource(std::string label)
            : m_label(std::move(label))
        {
        }

    protected:
        std::string m_label;
    };
} // namespace hyper_rhi