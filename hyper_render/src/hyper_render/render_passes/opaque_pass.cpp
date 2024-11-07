/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/render_passes/opaque_pass.hpp"

#include <hyper_core/filesystem.hpp>

#include "shader_interop.h"

namespace hyper_render
{
    OpaquePass::OpaquePass(
        const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device,
        const hyper_rhi::ShaderCompiler &shader_compiler,
        const std::shared_ptr<hyper_rhi::Texture> &render_texture,
        const std::shared_ptr<hyper_rhi::TextureView> &render_texture_view,
        const std::shared_ptr<hyper_rhi::Texture> &depth_texture,
        const std::shared_ptr<hyper_rhi::TextureView> &depth_texture_view)
        : m_render_texture(render_texture)
        , m_render_texture_view(render_texture_view)
        , m_depth_texture(depth_texture)
        , m_depth_texture_view(depth_texture_view)
        , m_pipeline_layout(graphics_device->create_pipeline_layout({
              .label = "Opaque",
              .push_constant_size = sizeof(ObjectPushConstants),
          }))
        , m_vertex_shader(graphics_device->create_shader_module({
              .label = "Opaque",
              .type = hyper_rhi::ShaderType::Vertex,
              .entry_name = "vs_main",
              .bytes = shader_compiler
                           .compile({
                               .type = hyper_rhi::ShaderType::Vertex,
                               .entry_name = "vs_main",
                               .data = hyper_core::filesystem::read_file("./assets/shaders/opaque_shader.hlsl"),
                           })
                           .spirv,
          }))
        , m_fragment_shader(graphics_device->create_shader_module({
              .label = "Opaque",
              .type = hyper_rhi::ShaderType::Fragment,
              .entry_name = "fs_main",
              .bytes = shader_compiler
                           .compile({
                               .type = hyper_rhi::ShaderType::Fragment,
                               .entry_name = "fs_main",
                               .data = hyper_core::filesystem::read_file("./assets/shaders/opaque_shader.hlsl"),
                           })
                           .spirv,
          }))
        , m_pipeline(graphics_device->create_render_pipeline({
              .label = "Opaque",
              .layout = m_pipeline_layout,
              .vertex_shader = m_vertex_shader,
              .fragment_shader = m_fragment_shader,
              .color_attachment_states = {
                  hyper_rhi::ColorAttachmentState{
                      .format = m_render_texture->format(),
                      .blend_state = hyper_rhi::BlendState {
                          .blend_enable = true,
                          .src_blend_factor = hyper_rhi::BlendFactor::SrcAlpha,
                          .dst_blend_factor = hyper_rhi::BlendFactor::OneMinusSrcAlpha,
                          .operation = hyper_rhi::BlendOperation::Add,
                          .alpha_src_blend_factor = hyper_rhi::BlendFactor::One,
                          .alpha_dst_blend_factor = hyper_rhi::BlendFactor::Zero,
                          .alpha_operation = hyper_rhi::BlendOperation::Add,
                          .color_writes = hyper_rhi::ColorWrites::All,
                      },
                  },
              },
              .primitive_state =
                  hyper_rhi::PrimitiveState{
                      .topology = hyper_rhi::PrimitiveTopology::TriangleList,
                      .front_face = hyper_rhi::FrontFace::CounterClockwise,
                      .cull_mode = hyper_rhi::Face::Back,
                      .polygon_mode = hyper_rhi::PolygonMode::Fill,
                  },
              .depth_stencil_state =
                  hyper_rhi::DepthStencilState{
                      .depth_test_enable = true,
                      .depth_write_enable = true,
                      .depth_format = m_depth_texture->format(),
                      .depth_compare_operation = hyper_rhi::CompareOperation::Less,
                      .depth_bias_state = {},
                  },
          }))
    {
    }

    void OpaquePass::render(const std::shared_ptr<hyper_rhi::CommandList> &command_list, const std::vector<std::shared_ptr<Mesh>> &meshes) const
    {
        const std::shared_ptr<hyper_rhi::RenderPass> render_pass = command_list->begin_render_pass({
            .label = "Opaque",
            .label_color =
                hyper_rhi::LabelColor{
                    .red = 254,
                    .green = 17,
                    .blue = 85,
                },
            .color_attachments = {
                hyper_rhi::ColorAttachment{
                    .view = m_render_texture_view,
                    .operation =
                        hyper_rhi::Operations{
                            .load_operation = hyper_rhi::LoadOperation::Clear,
                            .store_operation = hyper_rhi::StoreOperation::Store,
                        },
                },
            },
            .depth_stencil_attachment =
                hyper_rhi::DepthStencilAttachment{
                    .view = m_depth_texture_view,
                    .depth_operation =
                        hyper_rhi::Operations{
                            .load_operation = hyper_rhi::LoadOperation::Clear,
                            .store_operation = hyper_rhi::StoreOperation::Store,
                        },
                },
        });

        render_pass->set_pipeline(m_pipeline);

        for (const std::shared_ptr<Mesh> &mesh : meshes)
        {
            render_pass->set_index_buffer(mesh->indices_buffer);

            const ObjectPushConstants mesh_push_constants = {
                .mesh = mesh->mesh_buffer->handle(),
                .material = {},
                .padding_0 = 0,
                .padding_1 = 0,
            };
            render_pass->set_push_constants(&mesh_push_constants, sizeof(ObjectPushConstants));

            for (const auto &[start_index, count] : mesh->surfaces)
            {
                render_pass->draw_indexed(count, 1, start_index, 0, 0);
            }
        }
    }
} // namespace hyper_render