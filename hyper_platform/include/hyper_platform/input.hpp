/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <unordered_map>

#include <hyper_core/math.hpp>

#include "hyper_platform/forward.hpp"
#include "hyper_platform/key_codes.hpp"
#include "hyper_platform/mouse_codes.hpp"

namespace hyper_engine
{
    class Input
    {
    public:
        Input();

        // FIXME: Add unsubscription event handlers
        ~Input() = default;

        bool is_key_pressed(KeyCode) const;
        bool is_mouse_button_pressed(MouseCode) const;

        glm::vec2 mouse_position() const;

    private:
        void on_mouse_move(const MouseMoveEvent &);
        void on_mouse_button_press(const MouseButtonPressEvent &);
        void on_mouse_button_release(const MouseButtonReleaseEvent &);
        void on_key_press(const KeyPressEvent &);
        void on_key_release(const KeyReleaseEvent &);

    private:
        std::unordered_map<KeyCode, bool> m_keys;
        std::unordered_map<MouseCode, bool> m_mouse_buttons;
        glm::vec2 m_mouse_position = {0.0, 0.0};
    };
} // namespace hyper_engine