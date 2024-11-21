/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/material.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_core/filesystem.hpp>
#include <hyper_rhi/buffer.hpp>
#include <hyper_rhi/command_list.hpp>
#include <hyper_rhi/graphics_device.hpp>
#include <hyper_rhi/pipeline_layout.hpp>
#include <hyper_rhi/render_pipeline.hpp>
#include <hyper_rhi/sampler.hpp>
#include <hyper_rhi/shader_compiler.hpp>
#include <hyper_rhi/shader_module.hpp>
#include <hyper_rhi/texture.hpp>
#include <hyper_rhi/texture_view.hpp>

#include "shader_interop.h"

namespace he::render
{
    GltfMetallicRoughness::GltfMetallicRoughness(
        const std::shared_ptr<he::rhi::IGraphicsDevice> &graphics_device,
        const he::rhi::ShaderCompiler &shader_compiler,
        const std::shared_ptr<he::rhi::ITexture> &render_texture,
        const std::shared_ptr<he::rhi::ITexture> &depth_texture)
    {
        const std::shared_ptr<he::rhi::IShaderModule> vertex_shader = graphics_device->create_shader_module(
            {
                .label = "Mesh",
                .type = he::rhi::ShaderType::Vertex,
                .entry_name = "vs_main",
                .bytes = shader_compiler
                             .compile(
                                 {
                                     .type = he::rhi::ShaderType::Vertex,
                                     .entry_name = "vs_main",
                                     .data = he::core::fs::read_file("./assets/shaders/mesh_shader.hlsl"),
                                 })
                             .spirv,
            });

        const std::shared_ptr<he::rhi::IShaderModule> fragment_shader = graphics_device->create_shader_module(
            {
                .label = "Mesh",
                .type = he::rhi::ShaderType::Fragment,
                .entry_name = "fs_main",
                .bytes = shader_compiler
                             .compile(
                                 {
                                     .type = he::rhi::ShaderType::Fragment,
                                     .entry_name = "fs_main",
                                     .data = he::core::fs::read_file("./assets/shaders/mesh_shader.hlsl"),
                                 })
                             .spirv,
            });

        const std::shared_ptr<he::rhi::IPipelineLayout> pipeline_layout = graphics_device->create_pipeline_layout(
            {
                .label = "Mesh",
                .push_constant_size = sizeof(ObjectPushConstants),
            });

        m_opaque_pipeline = graphics_device->create_render_pipeline({
              .label = "Opaque",
              .layout = pipeline_layout,
              .vertex_shader = vertex_shader,
              .fragment_shader = fragment_shader,
              .color_attachment_states = {
                  he::rhi::ColorAttachmentState{
                      .format = render_texture->format(),
                      .blend_state = he::rhi::BlendState {
                          .blend_enable = false,
                          .src_blend_factor = he::rhi::BlendFactor::One,
                          .dst_blend_factor = he::rhi::BlendFactor::Zero,
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
                      .cull_mode = he::rhi::Face::Back,
                      .polygon_mode = he::rhi::PolygonMode::Fill,
                  },
              .depth_stencil_state =
                  he::rhi::DepthStencilState{
                      .depth_test_enable = true,
                      .depth_write_enable = true,
                      .depth_format = depth_texture->format(),
                      .depth_compare_operation = he::rhi::CompareOperation::Less,
                      .depth_bias_state = {},
                  },
          });

        m_transparent_pipeline = graphics_device->create_render_pipeline({
              .label = "Transparent",
              .layout = pipeline_layout,
              .vertex_shader = vertex_shader,
              .fragment_shader = fragment_shader,
              .color_attachment_states = {
                  he::rhi::ColorAttachmentState{
                      .format = render_texture->format(),
                      .blend_state = he::rhi::BlendState {
                          .blend_enable = true,
                          .src_blend_factor = he::rhi::BlendFactor::SrcAlpha,
                          .dst_blend_factor = he::rhi::BlendFactor::OneMinusSrcAlpha,
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
                      .cull_mode = he::rhi::Face::Back,
                      .polygon_mode = he::rhi::PolygonMode::Fill,
                  },
              .depth_stencil_state =
                  he::rhi::DepthStencilState{
                      .depth_test_enable = true,
                      .depth_write_enable = true,
                      // TODO: Add 2nd pass for transparent stuff
                      .depth_format = depth_texture->format(),
                      .depth_compare_operation = he::rhi::CompareOperation::Less,
                      .depth_bias_state = {},
                  },
          });
    }

    MaterialInstance GltfMetallicRoughness::write_material(
        const std::shared_ptr<he::rhi::IGraphicsDevice> &graphics_device,
        const std::shared_ptr<he::rhi::ICommandList> &command_list,
        const MaterialPassType pass_type,
        const MaterialResources &resources) const
    {
        const std::shared_ptr<he::rhi::IBuffer> buffer = graphics_device->create_buffer(
            {
                .label = "Material",
                .byte_size = sizeof(ShaderMaterial),
                .usage = he::rhi::BufferUsage::Storage | he::rhi::BufferUsage::ShaderResource,
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

        const std::shared_ptr<he::rhi::IRenderPipeline> pipeline = [&]()
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
} // namespace he::render