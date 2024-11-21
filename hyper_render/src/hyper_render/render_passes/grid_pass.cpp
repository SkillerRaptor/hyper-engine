/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/render_passes/grid_pass.hpp"

#include <hyper_core/filesystem.hpp>
#include <hyper_rhi/command_list.hpp>
#include <hyper_rhi/graphics_device.hpp>
#include <hyper_rhi/pipeline_layout.hpp>
#include <hyper_rhi/render_pass.hpp>
#include <hyper_rhi/render_pipeline.hpp>
#include <hyper_rhi/sampler.hpp>
#include <hyper_rhi/shader_compiler.hpp>
#include <hyper_rhi/shader_module.hpp>
#include <hyper_rhi/texture.hpp>
#include <hyper_rhi/texture_view.hpp>

namespace he::render
{
    GridPass::GridPass(
        const std::shared_ptr<he::rhi::IGraphicsDevice> &graphics_device,
        const he::rhi::ShaderCompiler &shader_compiler,
        const std::shared_ptr<he::rhi::ITexture> &render_texture,
        const std::shared_ptr<he::rhi::ITextureView> &render_texture_view,
        const std::shared_ptr<he::rhi::ITexture> &depth_texture,
        const std::shared_ptr<he::rhi::ITextureView> &depth_texture_view)
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
              .type = he::rhi::ShaderType::Vertex,
              .entry_name = "vs_main",
              .bytes = shader_compiler
                           .compile({
                               .type = he::rhi::ShaderType::Vertex,
                               .entry_name = "vs_main",
                               .data = he::core::fs::read_file("./assets/shaders/grid_shader.hlsl"),
                           })
                           .spirv,
          }))
        , m_fragment_shader(graphics_device->create_shader_module({
              .label = "Grid",
              .type = he::rhi::ShaderType::Fragment,
              .entry_name = "fs_main",
              .bytes = shader_compiler
                           .compile({
                               .type = he::rhi::ShaderType::Fragment,
                               .entry_name = "fs_main",
                               .data = he::core::fs::read_file("./assets/shaders/grid_shader.hlsl"),
                           })
                           .spirv,
          }))
        , m_pipeline(graphics_device->create_render_pipeline({
              .label = "Grid",
              .layout = m_pipeline_layout,
              .vertex_shader = m_vertex_shader,
              .fragment_shader = m_fragment_shader,
              .color_attachment_states = {
                  he::rhi::ColorAttachmentState{
                      .format = m_render_texture->format(),
                      .blend_state = he::rhi::BlendState {
                          .blend_enable = true,
                          .src_blend_factor = he::rhi::BlendFactor::SrcAlpha,
                          .dst_blend_factor = he::rhi::BlendFactor::One,
                          .operation = he::rhi::BlendOperation::Add,
                          .alpha_src_blend_factor = he::rhi::BlendFactor::One,
                          .alpha_dst_blend_factor = he::rhi::BlendFactor::Zero,
                          .alpha_operation = he::rhi::BlendOperation::Add,
                          .color_writes = he::rhi::ColorWrites::All,
                      },
                  },
              },
              .primitive_state =
                  he::rhi::PrimitiveState{
                      .topology = he::rhi::PrimitiveTopology::TriangleList,
                      .front_face = he::rhi::FrontFace::CounterClockwise,
                      .cull_mode = he::rhi::Face::None,
                      .polygon_mode = he::rhi::PolygonMode::Fill,
                  },
              .depth_stencil_state =
                  he::rhi::DepthStencilState{
                      .depth_test_enable = true,
                      .depth_write_enable = true,
                      .depth_format = m_depth_texture->format(),
                      .depth_compare_operation = he::rhi::CompareOperation::Less,
                      .depth_bias_state = {},
                  },
          }))
    {
    }

    void GridPass::render(const std::shared_ptr<he::rhi::ICommandList> &command_list) const
    {
        const std::shared_ptr<he::rhi::IRenderPass> render_pass = command_list->begin_render_pass({
            .label = "Grid",
            .label_color =
                he::rhi::LabelColor{
                    .red = 51,
                    .green = 187,
                    .blue = 255,
                },
            .color_attachments = {
                he::rhi::ColorAttachment{
                    .view = m_render_texture_view,
                    .operation =
                        he::rhi::Operations{
                            .load_operation = he::rhi::LoadOperation::Load,
                            .store_operation = he::rhi::StoreOperation::Store,
                        },
                },
            },
            .depth_stencil_attachment =
                he::rhi::DepthStencilAttachment{
                    .view = m_depth_texture_view,
                    .depth_operation =
                        he::rhi::Operations{
                            .load_operation = he::rhi::LoadOperation::Load,
                            .store_operation = he::rhi::StoreOperation::Store,
                        },
                },
        });

        render_pass->set_pipeline(m_pipeline);

        render_pass->draw(6, 1, 0, 0);
    }
} // namespace he::render