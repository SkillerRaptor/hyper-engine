/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/prerequisites.hpp>

#include "hyper_platform/mouse_codes.hpp"

namespace hyper_platform
{
    class MouseMoveEvent
    {
    public:
        MouseMoveEvent(const float x, const float y)
            : m_x(x)
            , m_y(y)
        {
        }

        [[nodiscard]] HE_FORCE_INLINE float x() const
        {
            return m_x;
        }

        [[nodiscard]] HE_FORCE_INLINE float y() const
        {
            return m_y;
        }

    private:
        float m_x;
        float m_y;
    };

    class MouseButtonPressEvent
    {
    public:
        explicit MouseButtonPressEvent(const MouseCode mouse_code)
            : m_mouse_code(mouse_code)
        {
        }

        [[nodiscard]] HE_FORCE_INLINE MouseCode mouse_code() const
        {
            return m_mouse_code;
        }

    private:
        MouseCode m_mouse_code;
    };

    class MouseButtonReleaseEvent
    {
    public:
        explicit MouseButtonReleaseEvent(const MouseCode mouse_code)
            : m_mouse_code(mouse_code)
        {
        }

        [[nodiscard]] HE_FORCE_INLINE MouseCode mouse_code() const
        {
            return m_mouse_code;
        }

    private:
        MouseCode m_mouse_code;
    };

    class MouseScrollEvent
    {
    public:
        MouseScrollEvent(const float delta_x, const float delta_y)
            : m_delta_x(delta_x)
            , m_delta_y(delta_y)
        {
        }

        [[nodiscard]] HE_FORCE_INLINE float delta_x() const
        {
            return m_delta_x;
        }

        [[nodiscard]] HE_FORCE_INLINE float delta_y() const
        {
            return m_delta_y;
        }

    private:
        float m_delta_x;
        float m_delta_y;
    };
} // namespace hyper_platform
