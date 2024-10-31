/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/renderer.hpp"

#include <array>

#include <fastgltf/core.hpp>
#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/tools.hpp>

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
        , m_depth_texture(m_graphics_device->create_texture({
              .label = "Swapchain Depth",
              .width = m_surface->width(),
              .height = m_surface->height(),
              .depth = 1,
              .array_size = 1,
              .mip_levels = 1,
              .format = hyper_rhi::TextureFormat::D32_SFloat,
              .dimension = hyper_rhi::TextureDimension::Texture2D,
              .usage = hyper_rhi::TextureUsage::RenderTarget,
          }))
        , m_shader_compiler()
        , m_queue(m_graphics_device->queue())
        , m_command_list(m_graphics_device->create_command_list())
        , m_pipeline_layout(m_graphics_device->create_pipeline_layout({
              .label = "Opaque",
              .push_constant_size = sizeof(ObjectPushConstants),
          }))
        , m_vertex_shader(m_graphics_device->create_shader_module({
              .label = "Opaque",
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
              .label = "Opaque",
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
              .label = "Opaque",
              .layout = m_pipeline_layout,
              .vertex_shader = m_vertex_shader,
                .fragment_shader = m_fragment_shader,
                .primitive_state = {
                    .cull_mode = hyper_rhi::Face::Back,
                },
              .depth_stencil_state = {
                  .depth_enabled = true,
                  .compare_operation = hyper_rhi::CompareOperation::Less,
              },
          }))
        , m_material_buffer(m_graphics_device->create_buffer({
              .label = "Material",
              .byte_size = sizeof(s_materials),
              .usage = hyper_rhi::BufferUsage::ShaderResource,
          }))
        , m_positions_buffer(m_graphics_device->create_buffer({
              .label = "Positions",
              .byte_size = sizeof(s_positions),
              .usage = hyper_rhi::BufferUsage::ShaderResource,
          }))
        , m_normals_buffer(m_graphics_device->create_buffer({
              .label = "Normals",
              .byte_size = sizeof(s_normals),
              .usage = hyper_rhi::BufferUsage::ShaderResource,
          }))
        , m_colors_buffer(m_graphics_device->create_buffer({
              .label = "Colors",
              .byte_size = sizeof(s_colors),
              .usage = hyper_rhi::BufferUsage::ShaderResource,
          }))
        , m_mesh_buffer(m_graphics_device->create_buffer({
              .label = "Mesh",
              .byte_size = sizeof(Mesh),
              .usage = hyper_rhi::BufferUsage::ShaderResource,
          }))
        , m_indices_buffer(m_graphics_device->create_buffer({
              .label = "Indices",
              .byte_size = sizeof(s_indices),
              .usage = hyper_rhi::BufferUsage::IndexBuffer,
          }))
        , m_camera_buffer(m_graphics_device->create_buffer({
              .label = "Camera",
              .byte_size = sizeof(::Camera),
              .usage = hyper_rhi::BufferUsage::ShaderResource,
              .handle = hyper_rhi::ResourceHandle(HE_DESCRIPTOR_SET_SLOT_CAMERA),
          }))
        , m_grid_pipeline_layout(m_graphics_device->create_pipeline_layout({
              .label = "Grid",
              // TODO: Test if size is 0
              .push_constant_size = 4,
          }))
        , m_grid_vertex_shader(m_graphics_device->create_shader_module({
              .label = "Grid",
              .type = hyper_rhi::ShaderType::Vertex,
              .entry_name = "vs_main",
              .bytes = m_shader_compiler
                           .compile({
                               .type = hyper_rhi::ShaderType::Vertex,
                               .entry_name = "vs_main",
                               .data = hyper_core::filesystem::read_file("./assets/shaders/grid_shader.hlsl"),
                           })
                           .spirv,
          }))
        , m_grid_fragment_shader(m_graphics_device->create_shader_module({
              .label = "Grid",
              .type = hyper_rhi::ShaderType::Fragment,
              .entry_name = "fs_main",
              .bytes = m_shader_compiler
                           .compile({
                               .type = hyper_rhi::ShaderType::Fragment,
                               .entry_name = "fs_main",
                               .data = hyper_core::filesystem::read_file("./assets/shaders/grid_shader.hlsl"),
                           })
                           .spirv,
          }))
        , m_grid_pipeline(m_graphics_device->create_graphics_pipeline({
              .label = "Grid",
              .layout = m_pipeline_layout,
              .vertex_shader = m_grid_vertex_shader,
              .fragment_shader = m_grid_fragment_shader,
              .primitive_state = {
                  .cull_mode = hyper_rhi::Face::None,
              },
              .depth_stencil_state = {
                  .depth_enabled = true,
                  .compare_operation = hyper_rhi::CompareOperation::Less,
              },
          }))
        , m_editor_camera(glm::vec3(0.0, 0.0, 0.0), -90.0, 0.0)
        , m_frame_index(1)
        , m_meshes()
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

        if (std::optional<std::vector<std::shared_ptr<MeshAsset>>> model = this->load_model("./assets/models/sponza/Sponza.gltf"))
        {
            m_meshes.insert(m_meshes.end(), model.value().begin(), model.value().end());
        }

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
            .inverse_view = glm::inverse(view_matrix),
            .projection = projection_matrix,
            .inverse_projection = glm::inverse(projection_matrix),
            .view_projection = view_projection,
            .inverse_view_projection = glm::inverse(view_projection),
            .near_plane = m_editor_camera.near_plane(),
            .far_plane = m_editor_camera.far_plane(),
            .padding_0 = 0.0,
            .padding_1 = 0.0,
        };

        m_queue->write_buffer(m_camera_buffer, &camera, sizeof(camera));
        m_queue->submit(nullptr);

        m_graphics_device->begin_frame(m_surface, m_frame_index);

        const std::shared_ptr<hyper_rhi::Texture> swapchain_texture = m_surface->current_texture();

        m_command_list->begin();

        m_command_list->insert_barriers({
            .memory_barriers = {},
            .buffer_memory_barriers = {},
            .texture_memory_barriers = {
                hyper_rhi::TextureMemoryBarrier {
                    .stage_before = hyper_rhi::BarrierPipelineStage::AllCommands,
                    .stage_after = hyper_rhi::BarrierPipelineStage::ColorAttachmentOutput,
                    .access_before = hyper_rhi::BarrierAccess::None,
                    .access_after = hyper_rhi::BarrierAccess::ColorAttachmentWrite,
                    .layout_before = hyper_rhi::BarrierTextureLayout::Undefined,
                    .layout_after = hyper_rhi::BarrierTextureLayout::ColorAttachment,
                    .texture = swapchain_texture,
                    .subresource_range = {
                        .base_mip_level = 0,
                        .mip_level_count = 1,
                        .base_array_level = 0,
                        .array_layer_count = 1,
                    },
                },
                hyper_rhi::TextureMemoryBarrier {
                    .stage_before = hyper_rhi::BarrierPipelineStage::AllCommands,
                    .stage_after = hyper_rhi::BarrierPipelineStage::LateFragmentTests,
                    .access_before = hyper_rhi::BarrierAccess::None,
                    .access_after = hyper_rhi::BarrierAccess::DepthStencilAttachmentWrite ,
                    .layout_before = hyper_rhi::BarrierTextureLayout::Undefined,
                    .layout_after = hyper_rhi::BarrierTextureLayout::DepthStencilAttachment,
                    .texture = m_depth_texture,
                    .subresource_range = {
                        .base_mip_level = 0,
                        .mip_level_count = 1,
                        .base_array_level = 0,
                        .array_layer_count = 1,
                    },
                },
            },
        });

        {
            const std::shared_ptr<hyper_rhi::RenderPass> render_pass = m_command_list->begin_render_pass({
                .label = "Opaque",
                .label_color = {
                    .red = 254,
                    .green = 17,
                    .blue = 85,
                },
                .color_attachment = {
                    .attachment = swapchain_texture,
                    .operation = {
                        .load_operation = hyper_rhi::LoadOperation::Clear,
                        .store_operation = hyper_rhi::StoreOperation::Store,
                    },
                },
                .depth_attachment = {
                    .attachment = m_depth_texture,
                    .operation = {
                        .load_operation = hyper_rhi::LoadOperation::Clear,
                        .store_operation = hyper_rhi::StoreOperation::Store,
                    },
                },
            });

            render_pass->set_pipeline(m_pipeline);

            /*
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
            */

            for (const std::shared_ptr<MeshAsset> &mesh : m_meshes)
            {
                render_pass->set_index_buffer(mesh->indices);

                const ObjectPushConstants mesh_push_constants = {
                    .mesh = mesh->mesh->handle(),
                    .material = m_material_buffer->handle(),
                    .padding_0 = 0,
                    .padding_1 = 0,
                };
                render_pass->set_push_constants(&mesh_push_constants, sizeof(ObjectPushConstants));

                for (const GeometrySurface &surface : mesh->surfaces)
                {
                    render_pass->draw_indexed({
                        .index_count = surface.count,
                        .instance_count = 1,
                        .first_index = surface.start_index,
                        .vertex_offset = 0,
                        .first_instance = 0,
                    });
                }
            }
        }

        m_command_list->insert_barriers({
            .memory_barriers = {},
            .buffer_memory_barriers = {},
            .texture_memory_barriers = {
                hyper_rhi::TextureMemoryBarrier {
                    .stage_before = hyper_rhi::BarrierPipelineStage::LateFragmentTests,
                    .stage_after = hyper_rhi::BarrierPipelineStage::EarlyFragmentTests,
                    .access_before = hyper_rhi::BarrierAccess::DepthStencilAttachmentWrite,
                    .access_after = hyper_rhi::BarrierAccess::DepthStencilAttachmentWrite,
                    .layout_before = hyper_rhi::BarrierTextureLayout::DepthStencilAttachment,
                    .layout_after = hyper_rhi::BarrierTextureLayout::DepthStencilAttachment,
                    .texture = m_depth_texture,
                    .subresource_range = {
                        .base_mip_level = 0,
                        .mip_level_count = 1,
                        .base_array_level = 0,
                        .array_layer_count = 1,
                    },
                },
            },
        });

        {
            const std::shared_ptr<hyper_rhi::RenderPass> render_pass = m_command_list->begin_render_pass({
                .label = "Grid",
                .label_color = {
                    .red = 51,
                    .green = 187,
                    .blue = 255,
                },
                .color_attachment = {
                    .attachment = swapchain_texture,
                    .operation = {
                        .load_operation = hyper_rhi::LoadOperation::Load,
                        .store_operation = hyper_rhi::StoreOperation::Store,
                    },
                },
                .depth_attachment = {
                    .attachment = m_depth_texture,
                    .operation = {
                        .load_operation = hyper_rhi::LoadOperation::Load,
                        .store_operation = hyper_rhi::StoreOperation::Store,
                    },
                },
            });

            render_pass->set_pipeline(m_grid_pipeline);

            render_pass->draw({
                .vertex_count = 6,
                .instance_count = 1,
                .first_vertex = 0,
                .first_instance = 0,
            });
        }

        m_command_list->insert_barriers({
            .memory_barriers = {},
            .buffer_memory_barriers = {},
            .texture_memory_barriers = {
                hyper_rhi::TextureMemoryBarrier {
                    .stage_before = hyper_rhi::BarrierPipelineStage::ColorAttachmentOutput,
                    .stage_after = hyper_rhi::BarrierPipelineStage::AllCommands,
                    .access_before = hyper_rhi::BarrierAccess::ColorAttachmentWrite,
                    .access_after = hyper_rhi::BarrierAccess::None,
                    .layout_before = hyper_rhi::BarrierTextureLayout::ColorAttachment,
                    .layout_after = hyper_rhi::BarrierTextureLayout::Present,
                    .texture = swapchain_texture,
                    .subresource_range = {
                        .base_mip_level = 0,
                        .mip_level_count = 1,
                        .base_array_level = 0,
                        .array_layer_count = 1,
                    },
                },
            },
        });

        m_command_list->end();

        m_graphics_device->end_frame();

        m_queue->submit(m_command_list);
        m_graphics_device->present(m_surface);

        m_frame_index += 1;
    }

    std::optional<std::vector<std::shared_ptr<MeshAsset>>> Renderer::load_model(const std::string &path) const
    {
        HE_INFO("Loading model: {}", path);

        const std::filesystem::path file_path(path);

        fastgltf::Parser parser;

        auto data = fastgltf::GltfDataBuffer::FromPath(file_path);
        if (data.error() != fastgltf::Error::None)
        {
            return std::nullopt;
        }

        auto asset = parser.loadGltf(data.get(), file_path.parent_path(), fastgltf::Options::LoadExternalBuffers);
        if (asset.error() != fastgltf::Error::None)
        {
            return std::nullopt;
        }

        std::vector<std::shared_ptr<MeshAsset>> meshes;

        struct Vertex
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec4 color;
        };

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        for (const fastgltf::Mesh &mesh : asset->meshes)
        {
            MeshAsset mesh_asset;
            mesh_asset.name = mesh.name;

            vertices.clear();
            indices.clear();

            for (const fastgltf::Primitive &primitive : mesh.primitives)
            {
                GeometrySurface surface;
                surface.start_index = static_cast<uint32_t>(indices.size());
                surface.count = static_cast<uint32_t>(asset->accessors[primitive.indicesAccessor.value()].count);

                const auto initial_vertex = static_cast<uint32_t>(vertices.size());

                {
                    fastgltf::Accessor &accessor = asset->accessors[primitive.indicesAccessor.value()];

                    fastgltf::iterateAccessor<uint32_t>(
                        asset.get(),
                        accessor,
                        [&](const uint32_t index)
                        {
                            indices.push_back(index + initial_vertex);
                        });
                }

                {
                    fastgltf::Accessor &accessor = asset->accessors[primitive.findAttribute("POSITION")->accessorIndex];
                    vertices.resize(vertices.size() + accessor.count);

                    fastgltf::iterateAccessorWithIndex<glm::vec3>(
                        asset.get(),
                        accessor,
                        [&](const glm::vec3 value, const size_t index)
                        {
                            const Vertex vertex = {
                                .position = value,
                                .normal = glm::vec3(1.0, 1.0, 1.0),
                                .color = glm::vec4(1.0, 1.0, 1.0, 1.0),
                            };

                            vertices[initial_vertex + index] = vertex;
                        });
                }

                const fastgltf::Attribute *normals = primitive.findAttribute("NORMAL");
                if (normals != primitive.attributes.end())
                {
                    fastgltf::iterateAccessorWithIndex<glm::vec3>(
                        asset.get(),
                        asset->accessors[normals->accessorIndex],
                        [&](const glm::vec3 value, const size_t index)
                        {
                            vertices[initial_vertex + index].normal = value;
                        });
                }

                const fastgltf::Attribute *colors = primitive.findAttribute("COLOR_0");
                if (colors != primitive.attributes.end())
                {
                    fastgltf::iterateAccessorWithIndex<glm::vec4>(
                        asset.get(),
                        asset->accessors[normals->accessorIndex],
                        [&](const glm::vec4 value, const size_t index)
                        {
                            vertices[initial_vertex + index].color = value;
                        });
                }

                mesh_asset.surfaces.push_back(surface);
            }

            float scale_size = 0.01f;
            glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(scale_size, scale_size, scale_size));

            std::vector<glm::vec4> positions;
            std::vector<glm::vec4> normals;
            std::vector<glm::vec4> colors;
            for (const Vertex &vertex : vertices)
            {
                glm::vec4 position = glm::vec4(-vertex.position.x, vertex.position.z, vertex.position.y, 1.0);
                position = scale * position;

                positions.emplace_back(position.x, position.y, position.z, 1.0);
                normals.emplace_back(-vertex.normal.x, vertex.normal.z, vertex.normal.y, 1.0);
                colors.emplace_back(vertex.color);
            }

            mesh_asset.positions = m_graphics_device->create_buffer({
                .label = fmt::format("{} Positions", mesh_asset.name),
                .byte_size = positions.size() * sizeof(glm::vec4),
                .usage = hyper_rhi::BufferUsage::ShaderResource,
            });
            m_queue->write_buffer(mesh_asset.positions, positions.data(), positions.size() * sizeof(glm::vec4));

            mesh_asset.normals = m_graphics_device->create_buffer({
                .label = fmt::format("{} Normals", mesh_asset.name),
                .byte_size = normals.size() * sizeof(glm::vec4),
                .usage = hyper_rhi::BufferUsage::ShaderResource,
            });
            m_queue->write_buffer(mesh_asset.normals, normals.data(), normals.size() * sizeof(glm::vec4));

            mesh_asset.colors = m_graphics_device->create_buffer({
                .label = fmt::format("{} Colors", mesh_asset.name),
                .byte_size = colors.size() * sizeof(glm::vec4),
                .usage = hyper_rhi::BufferUsage::ShaderResource,
            });
            m_queue->write_buffer(mesh_asset.colors, colors.data(), colors.size() * sizeof(glm::vec4));

            const Mesh mesh_data = {
                .positions = mesh_asset.positions->handle(),
                .normals = mesh_asset.normals->handle(),
                .colors = mesh_asset.colors->handle(),
                .padding_1 = 0,
            };
            mesh_asset.mesh = m_graphics_device->create_buffer({
                .label = fmt::format("{} Mesh Data", mesh_asset.name),
                .byte_size = sizeof(Mesh),
                .usage = hyper_rhi::BufferUsage::ShaderResource,
            });
            m_queue->write_buffer(mesh_asset.mesh, &mesh_data, sizeof(Mesh));

            for (size_t index = 0; index < indices.size(); index += 3)
            {
                std::swap(indices[index], indices[index + 2]);
            }

            mesh_asset.indices = m_graphics_device->create_buffer({
                .label = fmt::format("{} Indices", mesh_asset.name),
                .byte_size = indices.size() * sizeof(uint32_t),
                .usage = hyper_rhi::BufferUsage::IndexBuffer,
            });
            m_queue->write_buffer(mesh_asset.indices, indices.data(), indices.size() * sizeof(uint32_t));

            meshes.push_back(std::make_shared<MeshAsset>(std::move(mesh_asset)));
        }

        m_queue->submit(nullptr);

        return meshes;
    }

    void Renderer::on_resize(const hyper_platform::WindowResizeEvent &event)
    {
        m_editor_camera.set_aspect_ratio(static_cast<float>(event.width()) / static_cast<float>(event.height()));

        m_depth_texture = m_graphics_device->create_texture({
            .label = "Swapchain Depth",
            .width = event.width(),
            .height = event.height(),
            .depth = 1,
            .array_size = 1,
            .mip_levels = 1,
            .format = hyper_rhi::TextureFormat::D32_SFloat,
            .dimension = hyper_rhi::TextureDimension::Texture2D,
            .usage = hyper_rhi::TextureUsage::RenderTarget,
        });
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