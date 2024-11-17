/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>

namespace he::platform
{
    class WindowCloseEvent
    {
    public:
        WindowCloseEvent() = default;
    };

    class WindowMoveEvent
    {
    public:
        WindowMoveEvent(uint32_t x, uint32_t y);

        uint32_t x() const;
        uint32_t y() const;

    private:
        uint32_t m_x;
        uint32_t m_y;
    };

    class WindowResizeEvent
    {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height);

        uint32_t width() const;
        uint32_t height() const;

    private:
        uint32_t m_width;
        uint32_t m_height;
    };
} // namespace he::platform
