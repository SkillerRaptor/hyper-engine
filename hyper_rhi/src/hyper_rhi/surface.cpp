/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/surface.hpp"

#include <hyper_core/assertion.hpp>

namespace hyper_engine
{
    ISurface::ISurface(const uint32_t width, const uint32_t height)
        : m_resized(false)
        , m_width(width)
        , m_height(height)
    {
        HE_ASSERT(width > 0);
        HE_ASSERT(height > 0);
    }

    void ISurface::resize(const uint32_t width, const uint32_t height)
    {
        if (m_width == width || m_height == height)
        {
            return;
        }

        m_resized = true;
        m_width = width;
        m_height = height;
    }

    bool ISurface::resized() const
    {
        return m_resized;
    }

    uint32_t ISurface::width() const
    {
        return m_width;
    }

    uint32_t ISurface::height() const
    {
        return m_height;
    }
} // namespace hyper_engine