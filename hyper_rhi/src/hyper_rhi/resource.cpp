/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include <utility>

#include "hyper_rhi/resource.hpp"

namespace hyper_rhi
{
    Resource::Resource(std::string label)
        : m_label(std::move(label))
    {
    }

    std::string_view Resource::label() const
    {
        return m_label;
    }
} // namespace hyper_rhi
