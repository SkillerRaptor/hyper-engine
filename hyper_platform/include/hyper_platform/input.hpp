/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_event/event_bus.hpp"

#include <hyper_core/math.hpp>

#include "hyper_platform/key_codes.hpp"
#include "hyper_platform/key_events.hpp"
#include "hyper_platform/mouse_codes.hpp"

namespace hyper_platform
{
    class Input
    {
    public:
        static void initialize(hyper_event::EventBus &event_bus);

        [[nodiscard]] static bool is_key_pressed(KeyCode key_code);
        [[nodiscard]] static bool is_mouse_button_pressed(MouseCode mouse_code);

        [[nodiscard]] static glm::vec2 mouse_position();

    private:
        static void on_key_press(const KeyPressEvent &event);
        static void on_key_release(const KeyReleaseEvent &event);

    private:
        static std::unordered_map<KeyCode, bool> s_keys;
    };
} // namespace hyper_platform