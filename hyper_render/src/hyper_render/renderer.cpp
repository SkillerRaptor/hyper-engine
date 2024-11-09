/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/renderer.hpp"

#include <array>

#include <hyper_core/logger.hpp>
#include <hyper_core/prerequisites.hpp>

#include "hyper_render/material.hpp"

#include "shader_interop.h"

namespace hyper_render
{
    Renderer::Renderer(hyper_event::EventBus &event_bus, const hyper_platform::Input &input, const RendererDescriptor &descriptor)
        : m_input(input)
        , m_graphics_device(descriptor.graphics_device)
        , m_surface(descriptor.surface)
        , m_shader_compiler()
        , m_command_list(m_graphics_device->create_command_list())
        , m_render_texture(m_graphics_device->create_texture({
              .label = "Render",
              .width = m_surface->width(),
              .height = m_surface->height(),
              .depth = 1,
              .array_size = 1,
              .mip_levels = 1,
              .format = hyper_rhi::Format::Bgra8Unorm,
              .dimension = hyper_rhi::TextureDimension::Texture2D,
              .usage = hyper_rhi::TextureUsage::RenderAttachment,
          }))
        , m_render_texture_view(m_graphics_device->create_texture_view({
              .label = "Render",
              .texture = m_render_texture,
              .base_mip_level = 0,
              .mip_level_count = 1,
              .base_array_level = 0,
              .array_layer_count = 1,
              .component_mapping =
                  hyper_rhi::TextureComponentMapping{
                      .r = hyper_rhi::TextureComponentSwizzle::Identity,
                      .g = hyper_rhi::TextureComponentSwizzle::Identity,
                      .b = hyper_rhi::TextureComponentSwizzle::Identity,
                      .a = hyper_rhi::TextureComponentSwizzle::Identity,
                  },
              .format = hyper_rhi::Format::Bgra8Unorm,
              .dimension = hyper_rhi::TextureDimension::Texture2D,
          }))
        , m_depth_texture(m_graphics_device->create_texture({
              .label = "Depth",
              .width = m_surface->width(),
              .height = m_surface->height(),
              .depth = 1,
              .array_size = 1,
              .mip_levels = 1,
              .format = hyper_rhi::Format::D32Sfloat,
              .dimension = hyper_rhi::TextureDimension::Texture2D,
              .usage = hyper_rhi::TextureUsage::RenderAttachment,
          }))
        , m_depth_texture_view(m_graphics_device->create_texture_view({
              .label = "Depth",
              .texture = m_depth_texture,
              .base_mip_level = 0,
              .mip_level_count = 1,
              .base_array_level = 0,
              .array_layer_count = 1,
              .component_mapping =
                  hyper_rhi::TextureComponentMapping{
                      .r = hyper_rhi::TextureComponentSwizzle::Identity,
                      .g = hyper_rhi::TextureComponentSwizzle::Identity,
                      .b = hyper_rhi::TextureComponentSwizzle::Identity,
                      .a = hyper_rhi::TextureComponentSwizzle::Identity,
                  },
              .format = hyper_rhi::Format::D32Sfloat,
              .dimension = hyper_rhi::TextureDimension::Texture2D,
          }))
        , m_editor_camera(glm::vec3(0.0, 2.0, 0.0), -90.0, 0.0)
        , m_camera_buffer(m_graphics_device->create_buffer({
              .label = "Camera",
              .byte_size = sizeof(ShaderCamera),
              .usage = hyper_rhi::BufferUsage::Storage | hyper_rhi::BufferUsage::ShaderResource,
              .handle = hyper_rhi::ResourceHandle(HE_DESCRIPTOR_SET_SLOT_CAMERA),
          }))
        , m_scene_buffer(m_graphics_device->create_buffer({
              .label = "Scene",
              .byte_size = sizeof(ShaderScene),
              .usage = hyper_rhi::BufferUsage::Storage | hyper_rhi::BufferUsage::ShaderResource,
          }))
        , m_white_texture(m_graphics_device->create_texture({
              .label = "White",
              .width = 1,
              .height = 1,
              .depth = 1,
              .array_size = 1,
              .mip_levels = 1,
              .format = hyper_rhi::Format::Rgba8Unorm,
              .dimension = hyper_rhi::TextureDimension::Texture2D,
              .usage = hyper_rhi::TextureUsage::ShaderResource,
          }))
        , m_white_texture_view(m_graphics_device->create_texture_view({
              .label = "White",
              .texture = m_white_texture,
              .base_mip_level = 0,
              .mip_level_count = 1,
              .base_array_level = 0,
              .array_layer_count = 1,
              .component_mapping =
                  hyper_rhi::TextureComponentMapping{
                      .r = hyper_rhi::TextureComponentSwizzle::Identity,
                      .g = hyper_rhi::TextureComponentSwizzle::Identity,
                      .b = hyper_rhi::TextureComponentSwizzle::Identity,
                      .a = hyper_rhi::TextureComponentSwizzle::Identity,
                  },
              .format = hyper_rhi::Format::Rgba8Unorm,
              .dimension = hyper_rhi::TextureDimension::Texture2D,
          }))
        , m_default_sampler_nearest(m_graphics_device->create_sampler({
              .label = "Default Nearest",
              .mag_filter = hyper_rhi::Filter::Nearest,
              .min_filter = hyper_rhi::Filter::Nearest,
              .mipmap_filter = hyper_rhi::Filter::Nearest,
              .address_mode_u = hyper_rhi::SamplerAddressMode::Repeat,
              .address_mode_v = hyper_rhi::SamplerAddressMode::Repeat,
              .address_mode_w = hyper_rhi::SamplerAddressMode::Repeat,
              .mip_lod_bias = 0.0,
              .compare_operation = hyper_rhi::CompareOperation::Never,
              .min_lod = 0.0,
              .max_lod = 1.0,
              .border_color = hyper_rhi::BorderColor::TransparentBlack,
          }))
        , m_default_sampler_linear(m_graphics_device->create_sampler({
              .label = "Default Linear",
              .mag_filter = hyper_rhi::Filter::Linear,
              .min_filter = hyper_rhi::Filter::Linear,
              .mipmap_filter = hyper_rhi::Filter::Nearest,
              .address_mode_u = hyper_rhi::SamplerAddressMode::Repeat,
              .address_mode_v = hyper_rhi::SamplerAddressMode::Repeat,
              .address_mode_w = hyper_rhi::SamplerAddressMode::Repeat,
              .mip_lod_bias = 0.0,
              .compare_operation = hyper_rhi::CompareOperation::Never,
              .min_lod = 0.0,
              .max_lod = 1.0,
              .border_color = hyper_rhi::BorderColor::TransparentBlack,
          }))
        , m_metallic_roughness_material(m_graphics_device, m_shader_compiler, m_render_texture, m_depth_texture)
        , m_draw_context()
        , m_meshes()
        , m_loaded_nodes()
        , m_opaque_pass(nullptr)
        , m_grid_pass(nullptr)
        , m_frame_index(1)
    {
        event_bus.subscribe<hyper_platform::WindowResizeEvent>(HE_BIND_FUNCTION(Renderer::on_resize));
        event_bus.subscribe<hyper_platform::MouseMovedEvent>(HE_BIND_FUNCTION(Renderer::on_mouse_move));
        event_bus.subscribe<hyper_platform::MouseScrolledEvent>(HE_BIND_FUNCTION(Renderer::on_mouse_scroll));

        /*
        uint32_t white = glm::packUnorm4x8(glm::vec4(1, 1, 1, 1));
        _whiteImage = create_image((void *) &white, VkExtent3D{ 1, 1, 1 }, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT);

        uint32_t grey = glm::packUnorm4x8(glm::vec4(0.66f, 0.66f, 0.66f, 1));
        _greyImage = create_image((void *) &grey, VkExtent3D{ 1, 1, 1 }, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT);

        uint32_t black = glm::packUnorm4x8(glm::vec4(0, 0, 0, 0));
        _blackImage = create_image((void *) &black, VkExtent3D{ 1, 1, 1 }, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT);

        // checkerboard image
        uint32_t magenta = glm::packUnorm4x8(glm::vec4(1, 0, 1, 1));
        std::array<uint32_t, 16 * 16> pixels; // for 16x16 checkerboard texture
        for (int x = 0; x < 16; x++)
        {
            for (int y = 0; y < 16; y++)
            {
                pixels[y * 16 + x] = ((x % 2) ^ (y % 2)) ? magenta : black;
            }
        }
        _errorCheckerboardImage = create_image(pixels.data(), VkExtent3D{ 16, 16, 1 }, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT);
        */

        const GltfMetallicRoughness::MaterialResources material_resources = {
            .color_factors = glm::vec4(1.0, 1.0, 1.0, 1.0),
            .color_texture_view = m_white_texture_view,
            .color_sampler = m_default_sampler_linear,
            .metal_roughness_factors = glm::vec4(1.0, 0.5, 0.0, 0.0),
            .metal_roughness_texture_view = m_white_texture_view,
            .metal_roughness_sampler = m_default_sampler_linear,
        };

        m_command_list->begin();

        const uint32_t white = glm::packUnorm4x8(glm::vec4(1, 1, 1, 1));
        // TODO: Automate barrier
        m_command_list->insert_barriers(
        {
            .memory_barriers = {},
            .buffer_memory_barriers = {},
            .texture_memory_barriers = {
                hyper_rhi::TextureMemoryBarrier{
                    .stage_before = hyper_rhi::BarrierPipelineStage::AllCommands,
                    .stage_after = hyper_rhi::BarrierPipelineStage::AllCommands,
                    .access_before = hyper_rhi::BarrierAccess::None,
                    .access_after = hyper_rhi::BarrierAccess::TransferWrite,
                    .layout_before = hyper_rhi::BarrierTextureLayout::Undefined,
                    .layout_after = hyper_rhi::BarrierTextureLayout::TransferDst,
                    .texture = m_white_texture,
                    .subresource_range = hyper_rhi::TextureBarrierSubresourceRange{
                        .base_mip_level = 0,
                        .mip_level_count = 1,
                        .base_array_level = 0,
                        .array_layer_count = 1,
                    },
                },
            },
        });
        m_command_list->write_texture(
            m_white_texture,
            hyper_rhi::Offset3d{
                .x = 0,
                .y = 0,
                .z = 0,
            },
            hyper_rhi::Extent3d{
                .width = 1,
                .height = 1,
                .depth = 1,
            },
            0,
            0,
            &white,
            sizeof(white),
            0);

        MaterialInstance default_data =
            m_metallic_roughness_material.write_material(m_graphics_device, m_command_list, MaterialPassType::MainColor, material_resources);

        const std::vector<std::shared_ptr<Mesh>> model =
            Mesh::load_model(m_graphics_device, m_command_list, "./assets/models/sponza/Sponza.gltf");
        m_meshes.insert(m_meshes.end(), model.begin(), model.end());

        for (const std::shared_ptr<Mesh> &mesh : m_meshes)
        {
            auto node = std::make_shared<MeshNode>(glm::mat4(1.0f), glm::mat4(1.0f), mesh);

            for (Surface &surface : node->mesh().surfaces())
            {
                surface.material = std::make_shared<GltfMaterial>(default_data);
            }

            // m_loaded_nodes[std::string(mesh->name())] = std::move(node);
            m_loaded_nodes["Sponza"] = std::move(node);
        }

        m_command_list->end();

        m_graphics_device->execute(m_command_list);
        m_graphics_device->wait_for_idle();

        m_opaque_pass = std::make_unique<OpaquePass>(m_render_texture_view, m_depth_texture_view, m_scene_buffer);

        m_grid_pass = std::make_unique<GridPass>(
            m_graphics_device, m_shader_compiler, m_render_texture, m_render_texture_view, m_depth_texture, m_depth_texture_view);

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
        this->update_scene();

        m_graphics_device->begin_frame(m_surface, m_frame_index);

        m_command_list->begin();

        // Transition render texture to color attachment and depth texture to depth stencil attachment
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
                    .texture = m_render_texture,
                    .subresource_range = hyper_rhi::TextureBarrierSubresourceRange{
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
                    .subresource_range = hyper_rhi::TextureBarrierSubresourceRange{
                        .base_mip_level = 0,
                        .mip_level_count = 1,
                        .base_array_level = 0,
                        .array_layer_count = 1,
                    },
                },
            },
        });

        m_opaque_pass->render(m_command_list, m_draw_context);

        // NOTE: Ensure depth image was written
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
                    .subresource_range = hyper_rhi::TextureBarrierSubresourceRange{
                        .base_mip_level = 0,
                        .mip_level_count = 1,
                        .base_array_level = 0,
                        .array_layer_count = 1,
                    },
                },
            },
        });

        m_grid_pass->render(m_command_list);

        // TODO: Add error if current texture is asked before the frame began
        const std::shared_ptr<hyper_rhi::Texture> swapchain_texture = m_surface->current_texture();

        // NOTE: Transitioning the render and swapchain image for transfer
        m_command_list->insert_barriers(
        {
            .memory_barriers = {},
            .buffer_memory_barriers = {},
            .texture_memory_barriers = {
                hyper_rhi::TextureMemoryBarrier{
                    .stage_before = hyper_rhi::BarrierPipelineStage::ColorAttachmentOutput,
                    .stage_after = hyper_rhi::BarrierPipelineStage::AllTransfer,
                    .access_before = hyper_rhi::BarrierAccess::ColorAttachmentWrite,
                    .access_after = hyper_rhi::BarrierAccess::TransferRead,
                    .layout_before = hyper_rhi::BarrierTextureLayout::ColorAttachment,
                    .layout_after = hyper_rhi::BarrierTextureLayout::TransferSrc,
                    .texture = m_render_texture,
                    .subresource_range = hyper_rhi::TextureBarrierSubresourceRange{
                        .base_mip_level = 0,
                        .mip_level_count = 1,
                        .base_array_level = 0,
                        .array_layer_count = 1,
                    },
                },
                hyper_rhi::TextureMemoryBarrier{
                    .stage_before = hyper_rhi::BarrierPipelineStage::AllCommands,
                    .stage_after = hyper_rhi::BarrierPipelineStage::AllTransfer,
                    .access_before = hyper_rhi::BarrierAccess::None,
                    .access_after = hyper_rhi::BarrierAccess::TransferWrite,
                    .layout_before = hyper_rhi::BarrierTextureLayout::Undefined,
                    .layout_after = hyper_rhi::BarrierTextureLayout::TransferDst,
                    .texture = swapchain_texture,
                    .subresource_range = hyper_rhi::TextureBarrierSubresourceRange{
                        .base_mip_level = 0,
                        .mip_level_count = 1,
                        .base_array_level = 0,
                        .array_layer_count = 1,
                    },
                },
            },
        });

        m_command_list->copy_texture_to_texture(
            m_render_texture,
            hyper_rhi::Offset3d{
                .x = 0,
                .y = 0,
                .z = 0,
            },
            0,
            0,
            swapchain_texture,
            hyper_rhi::Offset3d{
                .x = 0,
                .y = 0,
                .z = 0,
            },
            0,
            0,
            hyper_rhi::Extent3d{
                .width = m_render_texture->width(),
                .height = m_render_texture->height(),
                .depth = m_render_texture->depth(),
            });

        m_command_list->insert_barriers(
        {
            .memory_barriers = {},
            .buffer_memory_barriers = {},
            .texture_memory_barriers = {
                hyper_rhi::TextureMemoryBarrier{
                    .stage_before = hyper_rhi::BarrierPipelineStage::AllTransfer,
                    .stage_after = hyper_rhi::BarrierPipelineStage::ColorAttachmentOutput,
                    .access_before = hyper_rhi::BarrierAccess::TransferWrite,
                    .access_after = hyper_rhi::BarrierAccess::ColorAttachmentWrite,
                    .layout_before = hyper_rhi::BarrierTextureLayout::TransferDst,
                    .layout_after = hyper_rhi::BarrierTextureLayout::ColorAttachment,
                    .texture = swapchain_texture,
                    .subresource_range = hyper_rhi::TextureBarrierSubresourceRange{
                        .base_mip_level = 0,
                        .mip_level_count = 1,
                        .base_array_level = 0,
                        .array_layer_count = 1,
                    },
                },
            },
        });

        // TODO: Draw GUI

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
                    .subresource_range = hyper_rhi::TextureBarrierSubresourceRange{
                        .base_mip_level = 0,
                        .mip_level_count = 1,
                        .base_array_level = 0,
                        .array_layer_count = 1,
                    },
                },
            },
        });

        /*
        m_command_list->insert_barriers(
        {
            .memory_barriers = {},
            .buffer_memory_barriers = {},
            .texture_memory_barriers = {
                hyper_rhi::TextureMemoryBarrier{
                    .stage_before = hyper_rhi::BarrierPipelineStage::AllTransfer,
                    .stage_after = hyper_rhi::BarrierPipelineStage::AllCommands,
                    .access_before = hyper_rhi::BarrierAccess::TransferWrite,
                    .access_after = hyper_rhi::BarrierAccess::None,
                    .layout_before = hyper_rhi::BarrierTextureLayout::TransferDst,
                    .layout_after = hyper_rhi::BarrierTextureLayout::Present,
                    .texture = swapchain_texture,
                    .subresource_range = hyper_rhi::TextureBarrierSubresourceRange{
                        .base_mip_level = 0,
                        .mip_level_count = 1,
                        .base_array_level = 0,
                        .array_layer_count = 1,
                    },
                },
            },
        });
        */

        m_command_list->end();

        m_graphics_device->end_frame();

        m_graphics_device->execute(m_command_list);
        m_graphics_device->present(m_surface);

        m_frame_index += 1;
    }

    void Renderer::create_textures(const uint32_t width, const uint32_t height)
    {
        m_render_texture = m_graphics_device->create_texture({
            .label = "Render",
            .width = width,
            .height = height,
            .depth = 1,
            .array_size = 1,
            .mip_levels = 1,
            .format = hyper_rhi::Format::Bgra8Unorm,
            .dimension = hyper_rhi::TextureDimension::Texture2D,
            .usage = hyper_rhi::TextureUsage::RenderAttachment,
        });

        m_render_texture_view = m_graphics_device->create_texture_view({
            .label = "Render",
            .texture = m_render_texture,
            .base_mip_level = 0,
            .mip_level_count = 1,
            .base_array_level = 0,
            .array_layer_count = 1,
            .component_mapping =
                hyper_rhi::TextureComponentMapping{
                    .r = hyper_rhi::TextureComponentSwizzle::Identity,
                    .g = hyper_rhi::TextureComponentSwizzle::Identity,
                    .b = hyper_rhi::TextureComponentSwizzle::Identity,
                    .a = hyper_rhi::TextureComponentSwizzle::Identity,
                },
            .format = hyper_rhi::Format::Bgra8Unorm,
            .dimension = hyper_rhi::TextureDimension::Texture2D,
        });

        m_depth_texture = m_graphics_device->create_texture({
            .label = "Depth",
            .width = width,
            .height = height,
            .depth = 1,
            .array_size = 1,
            .mip_levels = 1,
            .format = hyper_rhi::Format::D32Sfloat,
            .dimension = hyper_rhi::TextureDimension::Texture2D,
            .usage = hyper_rhi::TextureUsage::RenderAttachment,
        });

        m_depth_texture_view = m_graphics_device->create_texture_view({
            .label = "Depth",
            .texture = m_depth_texture,
            .base_mip_level = 0,
            .mip_level_count = 1,
            .base_array_level = 0,
            .array_layer_count = 1,
            .component_mapping =
                hyper_rhi::TextureComponentMapping{
                    .r = hyper_rhi::TextureComponentSwizzle::Identity,
                    .g = hyper_rhi::TextureComponentSwizzle::Identity,
                    .b = hyper_rhi::TextureComponentSwizzle::Identity,
                    .a = hyper_rhi::TextureComponentSwizzle::Identity,
                },
            .format = hyper_rhi::Format::D32Sfloat,
            .dimension = hyper_rhi::TextureDimension::Texture2D,
        });
    }

    void Renderer::update_scene()
    {
        // Upload buffers
        m_command_list->begin();

        const glm::mat4 view_matrix = m_editor_camera.view_matrix();
        const glm::mat4 projection_matrix = m_editor_camera.projection_matrix();
        const glm::mat4 view_projection = projection_matrix * view_matrix;
        const ShaderCamera shader_camera = {
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

        m_command_list->write_buffer(m_camera_buffer, &shader_camera, sizeof(ShaderCamera), 0);

        constexpr ShaderScene shader_scene = {
            .ambient_color = glm::vec4(0.1f),
            .sunlight_direction = glm::vec4(0.0f, 1.0f, 0.5f, 1.0f),
            .sunlight_color = glm::vec4(1.0f),
            .padding_0 = glm::vec4(0.0f),
        };

        m_command_list->write_buffer(m_scene_buffer, &shader_scene, sizeof(ShaderScene), 0);

        m_command_list->end();

        m_graphics_device->execute(m_command_list);
        m_graphics_device->wait_for_idle();

        // Draw meshes

        m_draw_context.opaque_surfaces.clear();

        const glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
        const glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0f));
        m_loaded_nodes["Sponza"]->draw(translation * scale, m_draw_context);
    }

    void Renderer::on_resize(const hyper_platform::WindowResizeEvent &event)
    {
        m_editor_camera.set_aspect_ratio(static_cast<float>(event.width()) / static_cast<float>(event.height()));

        this->create_textures(event.width(), event.height());
    }

    void Renderer::on_mouse_move(const hyper_platform::MouseMovedEvent &event)
    {
        m_editor_camera.process_mouse_movement(event.x(), event.y());
    }

    void Renderer::on_mouse_scroll(const hyper_platform::MouseScrolledEvent &event)
    {
        m_editor_camera.process_mouse_scroll(event.delta_y());
    }
} // namespace hyper_render