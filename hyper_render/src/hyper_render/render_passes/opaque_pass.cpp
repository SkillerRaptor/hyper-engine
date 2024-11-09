/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/render_passes/opaque_pass.hpp"

#include <hyper_core/filesystem.hpp>

#include "hyper_render/renderer.hpp"

#include "shader_interop.h"

namespace hyper_render
{
    OpaquePass::OpaquePass(
        const std::shared_ptr<hyper_rhi::TextureView> &render_texture_view,
        const std::shared_ptr<hyper_rhi::TextureView> &depth_texture_view,
        const std::shared_ptr<hyper_rhi::Buffer> &scene_buffer)
        : m_render_texture_view(render_texture_view)
        , m_depth_texture_view(depth_texture_view)
        , m_scene_buffer(scene_buffer)
    {
    }

    void OpaquePass::render(const std::shared_ptr<hyper_rhi::CommandList> &command_list, const DrawContext &draw_context) const
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

        for (const RenderObject &render_object : draw_context.opaque_surfaces)
        {
            render_pass->set_pipeline(render_object.material->pipeline);

            render_pass->set_index_buffer(render_object.index_buffer);

            const ObjectPushConstants mesh_push_constants = {
                .scene = m_scene_buffer->handle(),
                .mesh = render_object.mesh_buffer->handle(),
                .material = render_object.material->buffer->handle(),
                .padding_0 = 0,
                .transform_matrix = render_object.transform,
            };
            render_pass->set_push_constants(&mesh_push_constants, sizeof(ObjectPushConstants));

            render_pass->draw_indexed(render_object.index_count, 1, render_object.first_index, 0, 0);
        }
    }
} // namespace hyper_render