/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/renderer.hpp"

#include <array>

#include <hyper_core/logger.hpp>
#include <hyper_core/prerequisites.hpp>

#include "shader_interop.h"

namespace hyper_render
{
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
        , m_editor_camera(glm::vec3(0.0, 0.0, 0.0), -90.0, 0.0)
        , m_camera_buffer(m_graphics_device->create_buffer({
              .label = "Camera",
              .byte_size = sizeof(ShaderCamera),
              .usage = hyper_rhi::BufferUsage::ShaderResource,
              .handle = hyper_rhi::ResourceHandle(HE_DESCRIPTOR_SET_SLOT_CAMERA),
          }))
        , m_opaque_pass(std::make_unique<OpaquePass>(m_graphics_device, m_shader_compiler))
        , m_grid_pass(std::make_unique<GridPass>(m_graphics_device, m_shader_compiler))
        , m_meshes({})
        , m_frame_index(1)
    {
        event_bus.subscribe<hyper_platform::WindowResizeEvent>(HE_BIND_FUNCTION(Renderer::on_resize));
        event_bus.subscribe<hyper_platform::MouseMovedEvent>(HE_BIND_FUNCTION(Renderer::on_mouse_moved));
        event_bus.subscribe<hyper_platform::MouseScrolledEvent>(HE_BIND_FUNCTION(Renderer::on_mouse_scolled));

        if (std::optional<std::vector<std::shared_ptr<Mesh>>> model =
                Mesh::load_model(m_graphics_device, m_queue, "./assets/models/sponza/Sponza.gltf"))
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
        const ShaderCamera camera = {
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

        m_command_list->insert_barriers(
        {
            .memory_barriers = {},
            .buffer_memory_barriers = {},
            .texture_memory_barriers = {
                hyper_rhi::TextureMemoryBarrier{
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
                hyper_rhi::TextureMemoryBarrier{
                    .stage_before = hyper_rhi::BarrierPipelineStage::AllCommands,
                    .stage_after = hyper_rhi::BarrierPipelineStage::LateFragmentTests,
                    .access_before = hyper_rhi::BarrierAccess::None,
                    .access_after = hyper_rhi::BarrierAccess::DepthStencilAttachmentWrite,
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

        m_opaque_pass->render(m_command_list, swapchain_texture, m_depth_texture, m_meshes);

        m_command_list->insert_barriers(
        {
            .memory_barriers = {},
            .buffer_memory_barriers = {},
            .texture_memory_barriers = {
                hyper_rhi::TextureMemoryBarrier{
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

        m_grid_pass->render(m_command_list, swapchain_texture, m_depth_texture);

        m_command_list->insert_barriers(
        {
            .memory_barriers = {},
            .buffer_memory_barriers = {},
            .texture_memory_barriers = {
                hyper_rhi::TextureMemoryBarrier{
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