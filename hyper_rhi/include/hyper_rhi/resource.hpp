/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

namespace hyper_rhi
{
    class Resource
    {
    public:
        [[nodiscard]] std::string_view label() const;

    protected:
        explicit Resource(std::string label);

    protected:
        std::string m_label;
    };
} // namespace hyper_rhi