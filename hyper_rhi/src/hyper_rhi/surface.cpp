/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/surface.hpp"

#include <hyper_core/assertion.hpp>

namespace he::rhi
{
    Surface::Surface(const uint32_t width, const uint32_t height)
        : m_resized(false)
        , m_width(width)
        , m_height(height)
    {
        HE_ASSERT(width > 0);
        HE_ASSERT(height > 0);
    }

    void Surface::resize(const uint32_t width, const uint32_t height)
    {
        if (m_width == width || m_height == height)
        {
            return;
        }

        m_resized = true;
        m_width = width;
        m_height = height;
    }

    bool Surface::resized() const
    {
        return m_resized;
    }

    uint32_t Surface::width() const
    {
        return m_width;
    }

    uint32_t Surface::height() const
    {
        return m_height;
    }
} // namespace he::rhi