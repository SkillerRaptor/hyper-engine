/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_engine/editor_engine.hpp"

#include <hyper_core/global_environment.hpp>
#include <hyper_core/prerequisites.hpp>
#include <hyper_ecs/model_component.hpp>
#include <hyper_ecs/transform_component.hpp>
#include <hyper_event/event_bus.hpp>
#include <hyper_platform/input.hpp>
#include <hyper_platform/mouse_events.hpp>
#include <hyper_platform/window_events.hpp>

namespace hyper_engine
{
    bool EditorEngine::initialize()
    {
        g_env.event_bus->subscribe<WindowResizeEvent>(HE_BIND_FUNCTION(EditorEngine::on_resize));
        g_env.event_bus->subscribe<MouseMoveEvent>(HE_BIND_FUNCTION(EditorEngine::on_mouse_move));
        g_env.event_bus->subscribe<MouseScrollEvent>(HE_BIND_FUNCTION(EditorEngine::on_mouse_scroll));

        // FIXME: The editor shouldn't create entities on its own. Replace this after implementing project files
        entt::registry &registry = m_scene.registry();
        for (int32_t z = -10; z != 10; ++z)
        {
            for (int32_t x = -10; x != 10; ++x)
            {
                const entt::entity entity = registry.create();
                registry.emplace<TransformComponent>(
                    entity,
                    glm::vec3{static_cast<float>(x) * 2.0f, 1.0f, static_cast<float>(z) * 2.0f},
                    glm::vec3{0.0f, 0.0f, 0.0f},
                    glm::vec3{1.0f, 1.0f, 1.0f});
                // FIXME: Don't hardcode the model
                registry.emplace<ModelComponent>(entity, nullptr);
            }
        }

        // FIXME: Enable grid and gui

        return true;
    }

    void EditorEngine::shutdown()
    {
        // FIXME: Unsubscribe event listeners
    }

    void EditorEngine::fixed_update(const float delta_time, const float total_time)
    {
        (void) delta_time;
        (void) total_time;
    }

    void EditorEngine::update(const float delta_time, const float total_time)
    {
        (void) total_time;

        // FIXME: Add editor camera
        if (g_env.input->is_key_pressed(KeyCode::W))
        {
            m_camera.process_keyboard(Camera::Movement::Forward, delta_time);
        }

        if (g_env.input->is_key_pressed(KeyCode::S))
        {
            m_camera.process_keyboard(Camera::Movement::Backward, delta_time);
        }

        if (g_env.input->is_key_pressed(KeyCode::A))
        {
            m_camera.process_keyboard(Camera::Movement::Left, delta_time);
        }

        if (g_env.input->is_key_pressed(KeyCode::D))
        {
            m_camera.process_keyboard(Camera::Movement::Right, delta_time);
        }
    }

    void EditorEngine::render()
    {
        // FIXME: Draw debug lines and/or imgui
    }

    const Camera &EditorEngine::camera() const
    {
        return m_camera;
    }

    void EditorEngine::on_resize(const WindowResizeEvent &event)
    {
        m_camera.set_aspect_ratio(static_cast<float>(event.width()) / static_cast<float>(event.height()));
    }

    void EditorEngine::on_mouse_move(const MouseMoveEvent &event)
    {
        m_camera.process_mouse_movement(event.x(), event.y());
    }

    void EditorEngine::on_mouse_scroll(const MouseScrollEvent &event)
    {
        m_camera.process_mouse_scroll(event.delta_y());
    }
} // namespace hyper_engine