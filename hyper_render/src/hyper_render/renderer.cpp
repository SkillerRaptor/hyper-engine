/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/renderer.hpp"

#include <array>

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <hyper_core/filesystem.hpp>
#include <hyper_core/logger.hpp>
#include <hyper_core/prerequisites.hpp>
#include <hyper_rhi/render_pass.hpp>

#include "shader_interop.h"

namespace hyper_render
{
    struct Material
    {
        glm::vec4 base_color;
    };

    static constexpr std::array<Material, 1> s_materials = {
        Material{
            .base_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        },
    };

    static constexpr std::array<glm::vec4, 24> s_positions = {

        // Front face
        glm::vec4(-0.5f, -0.5f, 0.5f, 1.0),
        glm::vec4(0.5f, -0.5f, 0.5f, 1.0),
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0),
        glm::vec4(-0.5f, 0.5f, 0.5f, 1.0),

        // Back face
        glm::vec4(-0.5f, -0.5f, -0.5f, 1.0),
        glm::vec4(0.5f, -0.5f, -0.5f, 1.0),
        glm::vec4(0.5f, 0.5f, -0.5f, 1.0),
        glm::vec4(-0.5f, 0.5f, -0.5f, 1.0),

        // Right face
        glm::vec4(0.5f, -0.5f, 0.5f, 1.0),
        glm::vec4(0.5f, -0.5f, -0.5f, 1.0),
        glm::vec4(0.5f, 0.5f, -0.5f, 1.0),
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0),

        // Left face
        glm::vec4(-0.5f, -0.5f, 0.5f, 1.0),
        glm::vec4(-0.5f, -0.5f, -0.5f, 1.0),
        glm::vec4(-0.5f, 0.5f, -0.5f, 1.0),
        glm::vec4(-0.5f, 0.5f, 0.5f, 1.0),

        // Top face
        glm::vec4(-0.5f, 0.5f, 0.5f, 1.0),
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0),
        glm::vec4(0.5f, 0.5f, -0.5f, 1.0),
        glm::vec4(-0.5f, 0.5f, -0.5f, 1.0),

        // Bottom face
        glm::vec4(-0.5f, -0.5f, 0.5f, 1.0),
        glm::vec4(0.5f, -0.5f, 0.5f, 1.0),
        glm::vec4(0.5f, -0.5f, -0.5f, 1.0),
        glm::vec4(-0.5f, -0.5f, -0.5f, 1.0),
    };

    static constexpr std::array<glm::vec4, 24> s_normals = {
        glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0),
        glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0),
        glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0),
        glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0),
        glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0),
        glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0),
    };

    static constexpr std::array<glm::vec4, 24> s_colors = {
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0), // Front face (Red)
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0), // Front face (Red)
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0), // Front face (Red)
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0), // Front face (Red)

        glm::vec4(0.0f, 1.0f, 0.0f, 1.0), // Back face (Green)
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0), // Back face (Green)
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0), // Back face (Green)
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0), // Back face (Green)

        glm::vec4(0.0f, 0.0f, 1.0f, 1.0), // Right face (Blue)
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0), // Right face (Blue)
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0), // Right face (Blue)
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0), // Right face (Blue)

        glm::vec4(1.0f, 1.0f, 0.0f, 1.0), // Left face (Yellow)
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0), // Left face (Yellow)
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0), // Left face (Yellow)
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0), // Left face (Yellow)

        glm::vec4(1.0f, 0.0f, 1.0f, 1.0), // Top face (Magenta)
        glm::vec4(1.0f, 0.0f, 1.0f, 1.0), // Top face (Magenta)
        glm::vec4(1.0f, 0.0f, 1.0f, 1.0), // Top face (Magenta)
        glm::vec4(1.0f, 0.0f, 1.0f, 1.0), // Top face (Magenta)

        glm::vec4(0.0f, 1.0f, 1.0f, 1.0), // Bottom face (Cyan)
        glm::vec4(0.0f, 1.0f, 1.0f, 1.0), // Bottom face (Cyan)
        glm::vec4(0.0f, 1.0f, 1.0f, 1.0), // Bottom face (Cyan)
        glm::vec4(0.0f, 1.0f, 1.0f, 1.0), // Bottom face (Cyan)
    };

    static constexpr std::array<uint32_t, 36> s_indices = {
        // Front face
        0,
        1,
        2,
        2,
        3,
        0,
        // Back face
        4,
        5,
        6,
        6,
        7,
        4,
        // Right face
        8,
        9,
        10,
        10,
        11,
        8,
        // Left face
        12,
        13,
        14,
        14,
        15,
        12,
        // Top face
        16,
        17,
        18,
        18,
        19,
        16,
        // Bottom face
        20,
        21,
        22,
        22,
        23,
        20
    };

    Renderer::Renderer(hyper_event::EventBus &event_bus, const hyper_platform::Input &input, const RendererDescriptor &descriptor)
        : m_input(input)
        , m_graphics_device(descriptor.graphics_device)
        , m_surface(descriptor.surface)
        , m_shader_compiler()
        , m_queue(m_graphics_device->queue())
        , m_command_list(m_graphics_device->create_command_list())
        , m_pipeline_layout(m_graphics_device->create_pipeline_layout({
              .label = "Opaque Pipeline Layout",
              .push_constant_size = sizeof(ObjectPushConstants),
          }))
        , m_vertex_shader(m_graphics_device->create_shader_module({
              .label = "Opaque Vertex Shader",
              .type = hyper_rhi::ShaderType::Vertex,
              .entry_name = "vs_main",
              .bytes = m_shader_compiler
                           .compile({
                               .type = hyper_rhi::ShaderType::Vertex,
                               .entry_name = "vs_main",
                               .data = hyper_core::filesystem::read_file("./assets/shaders/opaque_shader.hlsl"),
                           })
                           .spirv,
          }))
        , m_fragment_shader(m_graphics_device->create_shader_module({
              .label = "Opaque Fragment Shader",
              .type = hyper_rhi::ShaderType::Fragment,
              .entry_name = "fs_main",
              .bytes = m_shader_compiler
                           .compile({
                               .type = hyper_rhi::ShaderType::Fragment,
                               .entry_name = "fs_main",
                               .data = hyper_core::filesystem::read_file("./assets/shaders/opaque_shader.hlsl"),
                           })
                           .spirv,
          }))
        , m_pipeline(m_graphics_device->create_graphics_pipeline({
              .label = "Opaque Pipeline",
              .layout = m_pipeline_layout,
              .vertex_shader = m_vertex_shader,
              .fragment_shader = m_fragment_shader,
          }))
        , m_material_buffer(m_graphics_device->create_buffer({
              .label = "Material Buffer",
              .byte_size = sizeof(s_materials),
              .is_index_buffer = false,
              .is_constant_buffer = true,
              .has_index = false,
          }))
        , m_positions_buffer(m_graphics_device->create_buffer({
              .label = "Positions Buffer",
              .byte_size = sizeof(s_positions),
              .is_index_buffer = false,
              .is_constant_buffer = true,
              .has_index = false,
          }))
        , m_normals_buffer(m_graphics_device->create_buffer({
              .label = "Normals Buffer",
              .byte_size = sizeof(s_normals),
              .is_index_buffer = false,
              .is_constant_buffer = true,
              .has_index = false,
          }))
        , m_colors_buffer(m_graphics_device->create_buffer({
              .label = "Colors Buffer",
              .byte_size = sizeof(s_colors),
              .is_index_buffer = false,
              .is_constant_buffer = true,
              .has_index = false,
          }))
        , m_mesh_buffer(m_graphics_device->create_buffer({
              .label = "Mesh Buffer",
              .byte_size = sizeof(Mesh) * 1,
              .is_index_buffer = false,
              .is_constant_buffer = true,
              .has_index = false,
          }))
        , m_indices_buffer(m_graphics_device->create_buffer({
              .label = "Indices Buffer",
              .byte_size = sizeof(s_indices),
              .is_index_buffer = true,
              .is_constant_buffer = false,
              .has_index = false,
          }))
        , m_camera_buffer(m_graphics_device->create_buffer({
              .label = "Camera Buffer",
              .byte_size = sizeof(::Camera),
              .is_index_buffer = false,
              .is_constant_buffer = true,
              .has_index = true,
          }))
        , m_editor_camera(glm::vec3(0.0, 0.0, 0.0), -90.0, 0.0)
        , m_frame_index(1)
    {
        event_bus.subscribe<hyper_platform::WindowResizeEvent>(HE_BIND_FUNCTION(Renderer::on_resize));
        event_bus.subscribe<hyper_platform::MouseMovedEvent>(HE_BIND_FUNCTION(Renderer::on_mouse_moved));
        event_bus.subscribe<hyper_platform::MouseScrolledEvent>(HE_BIND_FUNCTION(Renderer::on_mouse_scolled));

        m_queue->write_buffer(m_material_buffer, s_materials.data(), sizeof(s_materials));
        m_queue->write_buffer(m_positions_buffer, s_positions.data(), sizeof(s_positions));
        m_queue->write_buffer(m_normals_buffer, s_normals.data(), sizeof(s_normals));
        m_queue->write_buffer(m_colors_buffer, s_colors.data(), sizeof(s_colors));

        const Mesh mesh = {
            .positions = m_positions_buffer->handle(),
            .normals = m_normals_buffer->handle(),
            .colors = m_colors_buffer->handle(),
            .padding_1 = 0,
        };

        m_queue->write_buffer(m_mesh_buffer, &mesh, sizeof(Mesh));
        m_queue->write_buffer(m_indices_buffer, s_indices.data(), sizeof(s_indices));
        m_queue->submit(nullptr);

        m_graphics_device->set_dynamic_buffer(m_camera_buffer, HE_DESCRIPTOR_SET_SLOT_CAMERA);

        HE_INFO("Created Renderer");
    }

    void Renderer::update(const float delta_time)
    {
        if (m_input.is_key_pressed(hyper_platform::KeyCode::W))
        {
            m_editor_camera.process_keyboard(Camera::Movement::Forward, delta_time);
        }

        if (m_input.is_key_pressed(hyper_platform::KeyCode::S))
        {
            m_editor_camera.process_keyboard(Camera::Movement::Backward, delta_time);
        }

        if (m_input.is_key_pressed(hyper_platform::KeyCode::A))
        {
            m_editor_camera.process_keyboard(Camera::Movement::Left, delta_time);
        }

        if (m_input.is_key_pressed(hyper_platform::KeyCode::D))
        {
            m_editor_camera.process_keyboard(Camera::Movement::Right, delta_time);
        }
    }

    void Renderer::render()
    {
        const glm::mat4 view_matrix = m_editor_camera.view_matrix();
        const glm::mat4 projection_matrix = m_editor_camera.projection_matrix();
        const glm::mat4 view_projection = projection_matrix * view_matrix;
        const ::Camera camera = {
            .position = glm::vec4(m_editor_camera.position(), 1.0),
            .view = view_matrix,
            .projection = projection_matrix,
            .view_projection = view_projection,
            .inverse_view_projection = glm::inverse(view_projection),
        };

        m_queue->write_buffer(m_camera_buffer, &camera, sizeof(camera));
        m_queue->submit(nullptr);

        m_graphics_device->begin_frame(m_surface, m_frame_index);

        const hyper_rhi::TextureHandle swapchain_texture = m_surface->current_texture();

        m_command_list->begin();

        {
            const hyper_rhi::RenderPassHandle render_pass = m_command_list->begin_render_pass({
                .label = "Opaque Render Pass",
                .color_attachment = swapchain_texture,
            });

            render_pass->set_pipeline(m_pipeline);
            render_pass->set_index_buffer(m_indices_buffer);

            const ObjectPushConstants push_constants = {
                .mesh = m_mesh_buffer->handle(),
                .material = m_material_buffer->handle(),
                .padding_0 = 0,
                .padding_1 = 0,
            };
            render_pass->set_push_constants(&push_constants, sizeof(ObjectPushConstants));

            render_pass->draw_indexed({
                .index_count = static_cast<uint32_t>(s_indices.size()),
                .instance_count = 1,
                .first_index = 0,
                .vertex_offset = 0,
                .first_instance = 0,
            });
        }

        m_command_list->end();

        m_graphics_device->end_frame();

        m_queue->submit(m_command_list);
        m_graphics_device->present(m_surface);

        m_frame_index += 1;
    }

    void Renderer::on_resize(const hyper_platform::WindowResizeEvent &event)
    {
        m_editor_camera.set_aspect_ratio(static_cast<float>(event.width()) / static_cast<float>(event.height()));
    }

    void Renderer::on_mouse_moved(const hyper_platform::MouseMovedEvent &event)
    {
        m_editor_camera.process_mouse_movement(event.x(), event.y());
    }

    void Renderer::on_mouse_scolled(const hyper_platform::MouseScrolledEvent &event)
    {
        m_editor_camera.process_mouse_scroll(event.delta_y());
    }
} // namespace hyper_render