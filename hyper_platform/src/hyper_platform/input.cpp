/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_platform/input.hpp"

#include <SDL3/SDL.h>

namespace hyper_platform
{
    std::unordered_map<KeyCode, bool> Input::s_keys;

    void Input::initialize(hyper_event::EventBus &event_bus)
    {
        event_bus.subscribe<KeyPressEvent>(Input::on_key_press);
        event_bus.subscribe<KeyReleaseEvent>(Input::on_key_release);
    }

    bool Input::is_key_pressed(const KeyCode key_code)
    {
        return s_keys[key_code];
    }

    bool Input::is_mouse_button_pressed(const MouseCode mouse_code)
    {
        const SDL_MouseButtonFlags mouse_button_flags = SDL_GetMouseState(nullptr, nullptr);
        return (mouse_button_flags & SDL_BUTTON_MASK(static_cast<int32_t>(mouse_code))) != 0;
    }

    glm::vec2 Input::mouse_position()
    {
        float x = 0.0;
        float y = 0.0;
        SDL_GetMouseState(&x, &y);

        return { x, y };
    }

    void Input::on_key_press(const KeyPressEvent &event)
    {
        s_keys[event.key_code()] = true;
    }

    void Input::on_key_release(const KeyReleaseEvent &event)
    {
        s_keys[event.key_code()] = false;
    }
} // namespace hyper_platform