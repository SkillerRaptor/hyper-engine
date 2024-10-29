/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <glm/glm.hpp>

#include "hyper_platform/key_codes.hpp"
#include "hyper_platform/mouse_codes.hpp"
#include "hyper_platform/window.hpp"

namespace hyper_platform
{
    class Input
    {
    public:
        explicit Input(Window &window);

        bool is_key_pressed(KeyCode key_code) const;
        bool is_mouse_button_pressed(MouseCode mouse_code) const;

        glm::vec2 mouse_position() const;

    private:
        Window &m_window;
    };
} // namespace hyper_platform