/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

namespace he
{
    namespace rhi
    {
        class CommandList;
        class GraphicsDevice;
        class PipelineLayout;
        class RenderPipeline;
        class ShaderCompiler;
        class ShaderModule;
        class Texture;
        class TextureView;
    } // namespace rhi

    namespace render
    {
        class GridPass
        {
        public:
            GridPass(
                const std::shared_ptr<he::rhi::GraphicsDevice> &graphics_device,
                const he::rhi::ShaderCompiler &shader_compiler,
                const std::shared_ptr<he::rhi::Texture> &render_texture,
                const std::shared_ptr<he::rhi::TextureView> &render_texture_view,
                const std::shared_ptr<he::rhi::Texture> &depth_texture,
                const std::shared_ptr<he::rhi::TextureView> &depth_texture_view);

            void render(const std::shared_ptr<he::rhi::CommandList> &command_list) const;

        private:
            const std::shared_ptr<he::rhi::Texture> &m_render_texture;
            const std::shared_ptr<he::rhi::TextureView> &m_render_texture_view;
            const std::shared_ptr<he::rhi::Texture> &m_depth_texture;
            const std::shared_ptr<he::rhi::TextureView> &m_depth_texture_view;

            std::shared_ptr<he::rhi::PipelineLayout> m_pipeline_layout;
            std::shared_ptr<he::rhi::ShaderModule> m_vertex_shader;
            std::shared_ptr<he::rhi::ShaderModule> m_fragment_shader;
            std::shared_ptr<he::rhi::RenderPipeline> m_pipeline;
        };
    } // namespace render
} // namespace he