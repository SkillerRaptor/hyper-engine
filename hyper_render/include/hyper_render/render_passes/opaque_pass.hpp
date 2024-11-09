/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_rhi/graphics_device.hpp>
#include <hyper_rhi/shader_compiler.hpp>

#include "hyper_render/mesh.hpp"

namespace hyper_render
{
    struct DrawContext;

    class OpaquePass
    {
    public:
        OpaquePass(
            const std::shared_ptr<hyper_rhi::TextureView> &render_texture_view,
            const std::shared_ptr<hyper_rhi::TextureView> &depth_texture_view,
            const std::shared_ptr<hyper_rhi::Buffer> &scene_buffer);

        void render(const std::shared_ptr<hyper_rhi::CommandList> &command_list, const DrawContext &draw_context) const;

    private:
        const std::shared_ptr<hyper_rhi::TextureView> &m_render_texture_view;
        const std::shared_ptr<hyper_rhi::TextureView> &m_depth_texture_view;
        const std::shared_ptr<hyper_rhi::Buffer> &m_scene_buffer;
    };
} // namespace hyper_render