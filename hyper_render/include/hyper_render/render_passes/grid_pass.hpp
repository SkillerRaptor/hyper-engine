/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_rhi/graphics_device.hpp>
#include <hyper_rhi/shader_compiler.hpp>

namespace hyper_render
{
    class GridPass
    {
    public:
        GridPass(
            const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device,
            const hyper_rhi::ShaderCompiler &shader_compiler,
            const std::shared_ptr<hyper_rhi::Texture> &render_texture,
            const std::shared_ptr<hyper_rhi::TextureView> &render_texture_view,
            const std::shared_ptr<hyper_rhi::Texture> &depth_texture,
            const std::shared_ptr<hyper_rhi::TextureView> &depth_texture_view);

        void render(const std::shared_ptr<hyper_rhi::CommandList> &command_list) const;

    private:
        const std::shared_ptr<hyper_rhi::Texture> &m_render_texture;
        const std::shared_ptr<hyper_rhi::TextureView> &m_render_texture_view;
        const std::shared_ptr<hyper_rhi::Texture> &m_depth_texture;
        const std::shared_ptr<hyper_rhi::TextureView> &m_depth_texture_view;

        std::shared_ptr<hyper_rhi::PipelineLayout> m_pipeline_layout;
        std::shared_ptr<hyper_rhi::ShaderModule> m_vertex_shader;
        std::shared_ptr<hyper_rhi::ShaderModule> m_fragment_shader;
        std::shared_ptr<hyper_rhi::RenderPipeline> m_pipeline;
    };
} // namespace hyper_render