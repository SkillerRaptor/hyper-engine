/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_engine/camera.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_platform/input.hpp>
#include <hyper_platform/mouse_codes.hpp>

namespace hyper_engine
{
    Camera::Camera(const glm::vec3 position, const float yaw, const float pitch)
        : m_position(position)
        , m_yaw(yaw)
        , m_pitch(pitch)
    {
        update_camera_vectors();
    }

    void Camera::process_keyboard(const Movement movement, const float delta_time)
    {
        const float velocity = m_movement_speed * delta_time;
        switch (movement)
        {
        case Movement::Forward:
            m_position += m_front * velocity;
            break;
        case Movement::Backward:
            m_position -= m_front * velocity;
            break;
        case Movement::Left:
            m_position -= m_right * velocity;
            break;
        case Movement::Right:
            m_position += m_right * velocity;
            break;
        default:
            HE_UNREACHABLE();
        }
    }

    void Camera::process_mouse_movement(const float x_position, const float y_position)
    {
        if (m_first_mouse)
        {
            m_last_x = x_position;
            m_last_y = y_position;
            m_first_mouse = false;
        }

        const float x_offset = (x_position - m_last_x) * m_mouse_sensitivity;
        const float y_offset = (m_last_y - y_position) * m_mouse_sensitivity;

        m_last_x = x_position;
        m_last_y = y_position;

        if (!Input::get()->is_mouse_button_pressed(MouseCode::ButtonMiddle))
        {
            return;
        }

        m_yaw += x_offset;
        m_pitch += y_offset;

        if (m_pitch > 89.0f)
        {
            m_pitch = 89.0;
        }

        if (m_pitch < -89.0f)
        {
            m_pitch = -89.0;
        }

        update_camera_vectors();
    }

    void Camera::process_mouse_scroll(const float y_offset)
    {
        m_fov -= y_offset;
        if (m_fov < 1.0f)
        {
            m_fov = 1.0;
        }
        else if (m_fov > 90.0f)
        {
            m_fov = 90.0;
        }
    }

    void Camera::set_aspect_ratio(const float aspect_ratio)
    {
        m_aspect_ratio = aspect_ratio;
    }

    glm::vec3 Camera::position() const
    {
        return m_position;
    }

    float Camera::near_plane() const
    {
        return m_near;
    }

    float Camera::far_plane() const
    {
        return m_far;
    }

    glm::mat4 Camera::projection_matrix() const
    {
        return glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far);
    }

    glm::mat4 Camera::view_matrix() const
    {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    void Camera::update_camera_vectors()
    {
        const glm::vec3 front = {
            cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
            sin(glm::radians(m_pitch)),
            sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
        };

        m_front = glm::normalize(front);

        m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0, 1.0, 0.0)));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }
} // namespace hyper_engine