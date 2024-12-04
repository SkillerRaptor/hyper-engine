/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_platform/input.hpp"

#include <SDL3/SDL.h>

#include <hyper_core/global_environment.hpp>
#include <hyper_core/prerequisites.hpp>
#include <hyper_event/event_bus.hpp>

#include "hyper_platform/key_events.hpp"
#include "hyper_platform/mouse_events.hpp"

namespace hyper_engine
{
    Input::Input()
        : m_keys()
        , m_mouse_buttons()
        , m_mouse_position()
    {
        g_environment.event_bus->subscribe<MouseMoveEvent>(HE_BIND_FUNCTION(Input::on_mouse_move));
        g_environment.event_bus->subscribe<MouseButtonPressEvent>(HE_BIND_FUNCTION(Input::on_mouse_button_press));
        g_environment.event_bus->subscribe<MouseButtonReleaseEvent>(HE_BIND_FUNCTION(Input::on_mouse_button_release));
        g_environment.event_bus->subscribe<KeyPressEvent>(HE_BIND_FUNCTION(Input::on_key_press));
        g_environment.event_bus->subscribe<KeyReleaseEvent>(HE_BIND_FUNCTION(Input::on_key_release));
    }

    Input::~Input()
    {
        // TODO: Unsubscribe event handlers
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