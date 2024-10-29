/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/renderer.hpp"

#include "hyper_rhi/render_pass.hpp"

#include <array>

#include <glm/glm.hpp>

#include <hyper_core/filesystem.hpp>
#include <hyper_core/logger.hpp>

struct Material
{
    glm::vec4 base_color;
};

struct Mesh
{
    hyper_rhi::ResourceHandle positions;
    hyper_rhi::ResourceHandle normals;
    uint32_t padding_0;
    uint32_t padding_1;
};

struct ObjectPushConstants
{
    hyper_rhi::ResourceHandle mesh;
    hyper_rhi::ResourceHandle material;
    uint32_t padding_0;
    uint32_t padding_1;
};

namespace hyper_render
{
    static constexpr std::array<Material, 1> s_materials = {
        Material{
            .base_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        },
    };

    static constexpr std::array<glm::vec4, 4> s_positions = {
        glm::vec4(-0.5, -0.5, 0.0, 1.0),
        glm::vec4(0.5, -0.5, 0.0, 1.0),
        glm::vec4(0.5, 0.5, 0.0, 1.0),
        glm::vec4(-0.5, 0.5, 0.0, 1.0),
    };

    static constexpr std::array<glm::vec4, 4> s_normals = {
        glm::vec4(0.0, 0.0, 0.0, 0.0),
        glm::vec4(0.0, 0.0, 0.0, 0.0),
        glm::vec4(0.0, 0.0, 0.0, 0.0),
        glm::vec4(0.0, 0.0, 0.0, 0.0),
    };

    static constexpr std::array<uint32_t, 6> s_indices = {
        0, 1, 2, 2, 3, 0,
    };

    Renderer::Renderer(const RendererDescriptor &descriptor)
        : m_graphics_device(descriptor.graphics_device)
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
        , m_frame_index(1)
    {
        m_queue->write_buffer(m_material_buffer, s_materials.data(), sizeof(s_materials));
        m_queue->write_buffer(m_positions_buffer, s_positions.data(), sizeof(s_positions));
        m_queue->write_buffer(m_normals_buffer, s_normals.data(), sizeof(s_normals));

        const Mesh mesh = {
            .positions = m_positions_buffer->handle(),
            .normals = m_normals_buffer->handle(),
            .padding_0 = 0,
            .padding_1 = 0,
        };

        m_queue->write_buffer(m_mesh_buffer, &mesh, sizeof(Mesh));
        m_queue->write_buffer(m_indices_buffer, s_indices.data(), sizeof(s_indices));

        m_queue->submit(nullptr);

        HE_INFO("Created Renderer");
    }

    void Renderer::render()
    {
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
                .index_count = 6,
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
} // namespace hyper_render