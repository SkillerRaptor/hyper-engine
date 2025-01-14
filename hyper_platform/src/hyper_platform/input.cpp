/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_platform/input.hpp"

#include <hyper_core/prerequisites.hpp>
#include <hyper_event/event_bus.hpp>

#include "hyper_platform/key_events.hpp"
#include "hyper_platform/mouse_events.hpp"

namespace hyper_engine
{
    Input::Input()
    {
        EventBus::get()->subscribe<MouseMoveEvent>(HE_BIND_FUNCTION(Input::on_mouse_move));
        EventBus::get()->subscribe<MouseButtonPressEvent>(HE_BIND_FUNCTION(Input::on_mouse_button_press));
        EventBus::get()->subscribe<MouseButtonReleaseEvent>(HE_BIND_FUNCTION(Input::on_mouse_button_release));
        EventBus::get()->subscribe<KeyPressEvent>(HE_BIND_FUNCTION(Input::on_key_press));
        EventBus::get()->subscribe<KeyReleaseEvent>(HE_BIND_FUNCTION(Input::on_key_release));
    }

    bool Input::is_key_pressed(const KeyCode key_code) const
    {
        return m_keys.contains(key_code) ? m_keys.at(key_code) : false;
    }

    bool Input::is_mouse_button_pressed(const MouseCode mouse_code) const
    {
        return m_mouse_buttons.contains(mouse_code) ? m_mouse_buttons.at(mouse_code) : false;
    }

    glm::vec2 Input::mouse_position() const
    {
        return m_mouse_position;
    }

    Input *&Input::get()
    {
        static Input *input = nullptr;
        return input;
    }

    void Input::on_mouse_move(const MouseMoveEvent &event)
    {
        m_mouse_position = {event.x(), event.y()};
    }

    void Input::on_mouse_button_press(const MouseButtonPressEvent &event)
    {
        m_mouse_buttons[event.mouse_code()] = true;
    }

    void Input::on_mouse_button_release(const MouseButtonReleaseEvent &event)
    {
        m_mouse_buttons[event.mouse_code()] = false;
    }

    void Input::on_key_press(const KeyPressEvent &event)
    {
        m_keys[event.key_code()] = true;
    }

    void Input::on_key_release(const KeyReleaseEvent &event)
    {
        m_keys[event.key_code()] = false;
    }
} // namespace hyper_engine