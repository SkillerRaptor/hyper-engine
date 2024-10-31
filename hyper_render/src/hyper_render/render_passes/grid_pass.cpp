/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/render_passes/grid_pass.hpp"

#include <hyper_core/filesystem.hpp>

namespace hyper_render
{
    GridPass::GridPass(const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device, const hyper_rhi::ShaderCompiler &shader_compiler)
        : m_grid_pipeline_layout(graphics_device->create_pipeline_layout({
              .label = "Grid",
              // TODO: Test if size is 0
              .push_constant_size = 4,
          }))
        , m_grid_vertex_shader(graphics_device->create_shader_module({
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
        , m_grid_fragment_shader(graphics_device->create_shader_module({
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
        , m_grid_pipeline(graphics_device->create_graphics_pipeline({
              .label = "Grid",
              .layout = m_grid_pipeline_layout,
              .vertex_shader = m_grid_vertex_shader,
              .fragment_shader = m_grid_fragment_shader,
              .primitive_state =
                  hyper_rhi::PrimitiveState{
                      .cull_mode = hyper_rhi::Face::None,
                  },
              .depth_stencil_state =
                  hyper_rhi::DepthStencilState{
                      .depth_enabled = true,
                      .compare_operation = hyper_rhi::CompareOperation::Less,
                  },
          }))
    {
    }

    void GridPass::render(
        const std::shared_ptr<hyper_rhi::CommandList> &command_list,
        const std::shared_ptr<hyper_rhi::Texture> &swapchain_texture,
        const std::shared_ptr<hyper_rhi::Texture> &depth_texture) const
    {
        const std::shared_ptr<hyper_rhi::RenderPass> render_pass = command_list->begin_render_pass(
        {
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
                .attachment = depth_texture,
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
} // namespace hyper_render