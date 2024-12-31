/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

namespace hyper_engine
{
    class CommandList;
    class GraphicsDevice;
    class PipelineLayout;
    class RenderPipeline;
    class ShaderCompiler;
    class ShaderModule;
    class Texture;
    class TextureView;

    class GridPass
    {
    public:
        GridPass(
            const ShaderCompiler &shader_compiler,
            const std::shared_ptr<Texture> &render_texture,
            const std::shared_ptr<TextureView> &render_texture_view,
            const std::shared_ptr<Texture> &depth_texture,
            const std::shared_ptr<TextureView> &depth_texture_view);

        void render(const std::shared_ptr<CommandList> &command_list) const;

    private:
        const std::shared_ptr<Texture> &m_render_texture;
        const std::shared_ptr<TextureView> &m_render_texture_view;
        const std::shared_ptr<Texture> &m_depth_texture;
        const std::shared_ptr<TextureView> &m_depth_texture_view;

        std::shared_ptr<PipelineLayout> m_pipeline_layout;
        std::shared_ptr<ShaderModule> m_vertex_shader;
        std::shared_ptr<ShaderModule> m_fragment_shader;
        std::shared_ptr<RenderPipeline> m_pipeline;
    };
} // namespace hyper_engine