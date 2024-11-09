/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/render_passes/grid_pass.hpp"

#include <hyper_core/filesystem.hpp>

namespace hyper_render
{
    GridPass::GridPass(
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
              .label = "Grid",
              .push_constant_size = 0,
          }))
        , m_vertex_shader(graphics_device->create_shader_module({
              .label = "Grid",
              .type = hyper_rhi::ShaderType::Vertex,
              .entry_name = "vs_main",
              .bytes = shader_compiler
                           .compile({
                               .type = hyper_rhi::ShaderType::Vertex,
                               .entry_name = "vs_main",
                               .data = hyper_core::filesystem::read_file("./assets/shaders/grid_shader.hlsl"),
                           })
                           .spirv,
          }))
        , m_fragment_shader(graphics_device->create_shader_module({
              .label = "Grid",
              .type = hyper_rhi::ShaderType::Fragment,
              .entry_name = "fs_main",
              .bytes = shader_compiler
                           .compile({
                               .type = hyper_rhi::ShaderType::Fragment,
                               .entry_name = "fs_main",
                               .data = hyper_core::filesystem::read_file("./assets/shaders/grid_shader.hlsl"),
                           })
                           .spirv,
          }))
        , m_pipeline(graphics_device->create_render_pipeline({
              .label = "Grid",
              .layout = m_pipeline_layout,
              .vertex_shader = m_vertex_shader,
              .fragment_shader = m_fragment_shader,
              .color_attachment_states = {
                  hyper_rhi::ColorAttachmentState{
                      .format = m_render_texture->format(),
                      .blend_state = hyper_rhi::BlendState {
                          .blend_enable = true,
                          .src_blend_factor = hyper_rhi::BlendFactor::SrcAlpha,
                          .dst_blend_factor = hyper_rhi::BlendFactor::One,
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
                      .front_face = hyper_rhi::FrontFace::Clockwise,
                      .cull_mode = hyper_rhi::Face::None,
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

    void GridPass::render(const std::shared_ptr<hyper_rhi::CommandList> &command_list) const
    {
        const std::shared_ptr<hyper_rhi::RenderPass> render_pass = command_list->begin_render_pass({
            .label = "Grid",
            .label_color =
                hyper_rhi::LabelColor{
                    .red = 51,
                    .green = 187,
                    .blue = 255,
                },
            .color_attachments = {
                hyper_rhi::ColorAttachment{
                    .view = m_render_texture_view,
                    .operation =
                        hyper_rhi::Operations{
                            .load_operation = hyper_rhi::LoadOperation::Load,
                            .store_operation = hyper_rhi::StoreOperation::Store,
                        },
                },
            },
            .depth_stencil_attachment =
                hyper_rhi::DepthStencilAttachment{
                    .view = m_depth_texture_view,
                    .depth_operation =
                        hyper_rhi::Operations{
                            .load_operation = hyper_rhi::LoadOperation::Load,
                            .store_operation = hyper_rhi::StoreOperation::Store,
                        },
                },
        });

        render_pass->set_pipeline(m_pipeline);

        render_pass->draw(6, 1, 0, 0);
    }
} // namespace hyper_render