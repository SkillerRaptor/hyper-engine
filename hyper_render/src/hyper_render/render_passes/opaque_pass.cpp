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
    OpaquePass::OpaquePass(const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device, const hyper_rhi::ShaderCompiler &shader_compiler)
        : m_pipeline_layout(graphics_device->create_pipeline_layout({
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
        , m_pipeline(graphics_device->create_graphics_pipeline({
              .label = "Opaque",
              .layout = m_pipeline_layout,
              .vertex_shader = m_vertex_shader,
              .fragment_shader = m_fragment_shader,
              .primitive_state =
                  hyper_rhi::PrimitiveState{
                      .cull_mode = hyper_rhi::Face::Back,
                  },
              .depth_stencil_state =
                  hyper_rhi::DepthStencilState{
                      .depth_enabled = true,
                      .compare_operation = hyper_rhi::CompareOperation::Less,
                  },
          }))
    {
    }

    void OpaquePass::render(
        const std::shared_ptr<hyper_rhi::CommandList> &command_list,
        const std::shared_ptr<hyper_rhi::Texture> &swapchain_texture,
        const std::shared_ptr<hyper_rhi::Texture> &depth_texture,
        const std::vector<std::shared_ptr<Mesh>> &meshes) const
    {
        const std::shared_ptr<hyper_rhi::RenderPass> render_pass = command_list->begin_render_pass({
            .label = "Opaque",
            .label_color =
                hyper_rhi::LabelColor{
                    .red = 254,
                    .green = 17,
                    .blue = 85,
                },
            .color_attachment =
                hyper_rhi::ColorAttachment{
                    .attachment = swapchain_texture,
                    .operation =
                        hyper_rhi::Operations{
                            .load_operation = hyper_rhi::LoadOperation::Clear,
                            .store_operation = hyper_rhi::StoreOperation::Store,
                        },
                },
            .depth_attachment =
                hyper_rhi::DepthAttachment{
                    .attachment = depth_texture,
                    .operation =
                        hyper_rhi::Operations{
                            .load_operation = hyper_rhi::LoadOperation::Clear,
                            .store_operation = hyper_rhi::StoreOperation::Store,
                        },
                },
        });

        render_pass->set_pipeline(m_pipeline);

        for (const std::shared_ptr<Mesh> &mesh : meshes)
        {
            render_pass->set_index_buffer(mesh->indices);

            const ObjectPushConstants mesh_push_constants = {
                .mesh = mesh->mesh->handle(),
                .material = {},
                .padding_0 = 0,
                .padding_1 = 0,
            };
            render_pass->set_push_constants(&mesh_push_constants, sizeof(ObjectPushConstants));

            for (const auto &[start_index, count] : mesh->surfaces)
            {
                render_pass->draw_indexed({
                    .index_count = count,
                    .instance_count = 1,
                    .first_index = start_index,
                    .vertex_offset = 0,
                    .first_instance = 0,
                });
            }
        }
    }
} // namespace hyper_render