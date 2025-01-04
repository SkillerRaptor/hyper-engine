/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_platform/mouse_codes.hpp"

namespace hyper_engine
{
    class MouseButtonPressEvent
    {
    public:
        explicit MouseButtonPressEvent(MouseCode mouse_code);

        MouseCode mouse_code() const;

    private:
        MouseCode m_mouse_code;
    };

    class MouseButtonReleaseEvent
    {
    public:
        explicit MouseButtonReleaseEvent(MouseCode mouse_code);

        MouseCode mouse_code() const;

    private:
        MouseCode m_mouse_code;
    };

    class MouseMoveEvent
    {
    public:
        MouseMoveEvent(float x, float y);

        float x() const;
        float y() const;

    private:
        float m_x;
        float m_y;
    };

    class MouseScrollEvent
    {
    public:
        MouseScrollEvent(float delta_x, float delta_y);

        float delta_x() const;
        float delta_y() const;

    private:
        float m_delta_x;
        float m_delta_y;
    };
} // namespace hyper_engine
