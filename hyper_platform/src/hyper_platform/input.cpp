/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_platform/input.hpp"

#include <glfw/glfw3.h>

namespace hyper_platform
{
    Input::Input(Window &window)
        : m_window(window)
    {
    }

    bool Input::is_key_pressed(KeyCode key_code) const
    {
        const int32_t state = glfwGetKey(m_window.native_window(), static_cast<int32_t>(key_code));
        return state == GLFW_PRESS;
    }

    bool Input::is_mouse_button_pressed(MouseCode mouse_code) const
    {
        const int32_t state = glfwGetMouseButton(m_window.native_window(), static_cast<int32_t>(mouse_code));
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::mouse_position() const
    {
        double x_position = 0.0;
        double y_position = 0.0;
        glfwGetCursorPos(m_window.native_window(), &x_position, &y_position);

        return { static_cast<float>(x_position), static_cast<float>(y_position) };
    }
} // namespace hyper_platform