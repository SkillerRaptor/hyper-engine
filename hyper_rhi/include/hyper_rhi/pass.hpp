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
    struct LabelColor
    {
        uint8_t red = 255;
        uint8_t green = 255;
        uint8_t blue = 255;
    };

    class Pass
    {
    public:
        [[nodiscard]] HE_FORCE_INLINE std::string_view label() const
        {
            return m_label;
        }

        [[nodiscard]] HE_FORCE_INLINE const LabelColor &label_color() const
        {
            return m_label_color;
        }

    protected:
        Pass(std::string label, const LabelColor label_color)
            : m_label(std::move(label))
              , m_label_color(label_color)
        {
        }

    protected:
        std::string m_label;
        LabelColor m_label_color;
    };
}