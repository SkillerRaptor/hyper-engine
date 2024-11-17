/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_platform/input.hpp"

#include <SDL3/SDL.h>

#include <hyper_event/event_bus.hpp>

#include "hyper_platform/key_events.hpp"

namespace he::platform::input
{
    static std::unordered_map<KeyCode, bool> g_keys;

    void on_key_press(const KeyPressEvent &event)
    {
        g_keys[event.key_code()] = true;
    }

    void on_key_release(const KeyReleaseEvent &event)
    {
        g_keys[event.key_code()] = false;
    }

    void initialize(he::event::EventBus &event_bus)
    {
        event_bus.subscribe<KeyPressEvent>(on_key_press);
        event_bus.subscribe<KeyReleaseEvent>(on_key_release);
    }

    bool is_key_pressed(const KeyCode key_code)
    {
        return g_keys[key_code];
    }

    bool is_mouse_button_pressed(const MouseCode mouse_code)
    {
        const SDL_MouseButtonFlags mouse_button_flags = SDL_GetMouseState(nullptr, nullptr);
        return (mouse_button_flags & SDL_BUTTON_MASK(static_cast<int32_t>(mouse_code))) != 0;
    }

    glm::vec2 mouse_position()
    {
        float x = 0.0;
        float y = 0.0;
        SDL_GetMouseState(&x, &y);

        return { x, y };
    }
} // namespace he::platform::input