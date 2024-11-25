/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <unordered_map>

#include <hyper_core/math.hpp>

#include "hyper_platform/key_codes.hpp"
#include "hyper_platform/mouse_codes.hpp"

namespace hyper_engine
{
    class EventBus;
    class MouseMoveEvent;
    class MouseButtonPressEvent;
    class MouseButtonReleaseEvent;
    class KeyPressEvent;
    class KeyReleaseEvent;

    class Input
    {
    public:
        Input();
        ~Input();

        bool is_key_pressed(KeyCode key_code) const;
        bool is_mouse_button_pressed(MouseCode mouse_code) const;

        glm::vec2 mouse_position() const;

    private:
        void on_mouse_move(const MouseMoveEvent &event);
        void on_mouse_button_press(const MouseButtonPressEvent &event);
        void on_mouse_button_release(const MouseButtonReleaseEvent &event);
        void on_key_press(const KeyPressEvent &event);
        void on_key_release(const KeyReleaseEvent &event);

    private:
        std::unordered_map<KeyCode, bool> m_keys;
        std::unordered_map<MouseCode, bool> m_mouse_buttons;
        glm::vec2 m_mouse_position;
    };
} // namespace hyper_engine