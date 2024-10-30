/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/surface.hpp"

namespace hyper_rhi
{
    Surface::Surface(const uint32_t width, const uint32_t height)
        : m_resized(false)
        , m_width(width)
        , m_height(height)
    {
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

    uint32_t Surface::width() const
    {
        return m_width;
    }

    uint32_t Surface::height() const
    {
        return m_height;
    }
} // namespace hyper_rhi