/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/math.hpp>

#include "hyper_platform/key_codes.hpp"
#include "hyper_platform/mouse_codes.hpp"

namespace hyper_engine
{
    class EventBus;

    namespace input
    {
        void initialize(EventBus &event_bus);

        bool is_key_pressed(KeyCode key_code);
        bool is_mouse_button_pressed(MouseCode mouse_code);

        glm::vec2 mouse_position();
    } // namespace input
} // namespace hyper_engine