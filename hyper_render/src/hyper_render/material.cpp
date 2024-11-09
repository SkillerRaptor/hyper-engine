/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/material.hpp"

#include <hyper_core/filesystem.hpp>

#include "shader_interop.h"

namespace hyper_render
{
    GltfMetallicRoughness::GltfMetallicRoughness(
        const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device,
        const hyper_rhi::ShaderCompiler &shader_compiler,
        const std::shared_ptr<hyper_rhi::Texture> &render_texture,
        const std::shared_ptr<hyper_rhi::Texture> &depth_texture)
    {
        const std::shared_ptr<hyper_rhi::ShaderModule> vertex_shader = graphics_device->create_shader_module({
            .label = "Mesh",
            .type = hyper_rhi::ShaderType::Vertex,
            .entry_name = "vs_main",
            .bytes = shader_compiler
                         .compile({
                             .type = hyper_rhi::ShaderType::Vertex,
                             .entry_name = "vs_main",
                             .data = hyper_core::filesystem::read_file("./assets/shaders/mesh_shader.hlsl"),
                         })
                         .spirv,
        });

        const std::shared_ptr<hyper_rhi::ShaderModule> fragment_shader = graphics_device->create_shader_module({
            .label = "Mesh",
            .type = hyper_rhi::ShaderType::Fragment,
            .entry_name = "fs_main",
            .bytes = shader_compiler
                         .compile({
                             .type = hyper_rhi::ShaderType::Fragment,
                             .entry_name = "fs_main",
                             .data = hyper_core::filesystem::read_file("./assets/shaders/mesh_shader.hlsl"),
                         })
                         .spirv,
        });

        const std::shared_ptr<hyper_rhi::PipelineLayout> pipeline_layout = graphics_device->create_pipeline_layout({
            .label = "Mesh",
            .push_constant_size = sizeof(ObjectPushConstants),
        });

        m_opaque_pipeline = graphics_device->create_render_pipeline({
              .label = "Opaque",
              .layout = pipeline_layout,
              .vertex_shader = vertex_shader,
              .fragment_shader = fragment_shader,
              .color_attachment_states = {
                  hyper_rhi::ColorAttachmentState{
                      .format = render_texture->format(),
                      .blend_state = hyper_rhi::BlendState {
                          .blend_enable = false,
                          .src_blend_factor = hyper_rhi::BlendFactor::One,
                          .dst_blend_factor = hyper_rhi::BlendFactor::Zero,
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
                      .cull_mode = hyper_rhi::Face::Back,
                      .polygon_mode = hyper_rhi::PolygonMode::Fill,
                  },
              .depth_stencil_state =
                  hyper_rhi::DepthStencilState{
                      .depth_test_enable = true,
                      .depth_write_enable = true,
                      .depth_format = depth_texture->format(),
                      .depth_compare_operation = hyper_rhi::CompareOperation::Less,
                      .depth_bias_state = {},
                  },
          });

        m_transparent_pipeline = graphics_device->create_render_pipeline({
              .label = "Transparent",
              .layout = pipeline_layout,
              .vertex_shader = vertex_shader,
              .fragment_shader = fragment_shader,
              .color_attachment_states = {
                  hyper_rhi::ColorAttachmentState{
                      .format = render_texture->format(),
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
                      .front_face = hyper_rhi::FrontFace::Clockwise,
                      .cull_mode = hyper_rhi::Face::Back,
                      .polygon_mode = hyper_rhi::PolygonMode::Fill,
                  },
              .depth_stencil_state =
                  hyper_rhi::DepthStencilState{
                      .depth_test_enable = true,
                      .depth_write_enable = true,
                      // TODO: Add 2nd pass for transparent stuff
                      .depth_format = depth_texture->format(),
                      .depth_compare_operation = hyper_rhi::CompareOperation::Less,
                      .depth_bias_state = {},
                  },
          });
    }

    MaterialInstance GltfMetallicRoughness::write_material(
        const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device,
        const std::shared_ptr<hyper_rhi::CommandList> &command_list,
        const MaterialPassType pass_type,
        const MaterialResources &resources) const
    {
        const std::shared_ptr<hyper_rhi::Buffer> buffer = graphics_device->create_buffer({
            .label = "Material",
            .byte_size = sizeof(ShaderMaterial),
            .usage = hyper_rhi::BufferUsage::Storage | hyper_rhi::BufferUsage::ShaderResource,
        });

        const ShaderMaterial shader_material = {
            .color_factors = resources.color_factors,
            .color_texture = resources.color_texture_view->handle(),
            .color_sampler = resources.color_sampler->handle(),
            .padding_0 = 0,
            .padding_1 = 0,
            .metal_roughness_factors = resources.metal_roughness_factors,
            .metal_roughness_texture = resources.metal_roughness_texture_view->handle(),
            .metal_roughness_sampler = resources.metal_roughness_sampler->handle(),
            .padding_2 = 0,
            .padding_3 = 0,
        };

        command_list->write_buffer(buffer, &shader_material, sizeof(ShaderMaterial), 0);

        const std::shared_ptr<hyper_rhi::RenderPipeline> pipeline = [&]()
        {
            switch (pass_type)
            {
            case MaterialPassType::MainColor:
                return m_opaque_pipeline;
            case MaterialPassType::Transparent:
                return m_transparent_pipeline;
            default:
                HE_UNREACHABLE();
            }
        }();

        const MaterialInstance material_instance = {
            .pipeline = pipeline,
            .pass_type = pass_type,
            .buffer = buffer,
        };

        return material_instance;
    }
} // namespace hyper_render