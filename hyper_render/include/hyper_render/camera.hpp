/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/math.hpp>

namespace hyper_render
{
    class Camera
    {
    public:
        enum class Movement
        {
            Forward,
            Backward,
            Left,
            Right,
        };

    public:
        Camera(glm::vec3 position, float yaw, float pitch);

        void process_keyboard(Movement movement, float delta_time);
        void process_mouse_movement(float x_position, float y_position, bool middle_mouse);
        void process_mouse_scroll(float y_offset);

        void set_aspect_ratio(float aspect_ratio);

        [[nodiscard]] glm::vec3 position() const;

        [[nodiscard]] float near_plane() const;
        [[nodiscard]] float far_plane() const;

        [[nodiscard]] glm::mat4 projection_matrix() const;
        [[nodiscard]] glm::mat4 view_matrix() const;

    private:
        void update_camera_vectors();

    private:
        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;

        float m_yaw;
        float m_pitch;

        float m_movement_speed;
        float m_mouse_sensitivity;
        float m_fov;

        float m_near;
        float m_far;
        float m_aspect_ratio;

        bool m_first_mouse;
        float m_last_x;
        float m_last_y;
    };
} // namespace hyper_render