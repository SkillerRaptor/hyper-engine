/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>

#include <hyper_core/prerequisites.hpp>

namespace hyper_platform
{
    class WindowCloseEvent
    {
    public:
        WindowCloseEvent() = default;
    };

    class WindowMoveEvent
    {
    public:
        WindowMoveEvent(const uint32_t x, const uint32_t y)
            : m_x(x)
            , m_y(y)
        {
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t x() const
        {
            return m_x;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t y() const
        {
            return m_y;
        }

    private:
        uint32_t m_x;
        uint32_t m_y;
    };

    class WindowResizeEvent
    {
    public:
        WindowResizeEvent(const uint32_t width, const uint32_t height)
            : m_width(width)
            , m_height(height)
        {
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t width() const
        {
            return m_width;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t height() const
        {
            return m_height;
        }

    private:
        uint32_t m_width;
        uint32_t m_height;
    };
} // namespace hyper_platform
