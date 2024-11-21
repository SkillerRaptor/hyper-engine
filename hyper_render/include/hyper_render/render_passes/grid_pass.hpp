/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

namespace hyper_engine
{
    class ICommandList;
    class IGraphicsDevice;
    class IPipelineLayout;
    class IRenderPipeline;
    class ShaderCompiler;
    class IShaderModule;
    class ITexture;
    class ITextureView;

    class GridPass
    {
    public:
        GridPass(
            const std::shared_ptr<IGraphicsDevice> &graphics_device,
            const ShaderCompiler &shader_compiler,
            const std::shared_ptr<ITexture> &render_texture,
            const std::shared_ptr<ITextureView> &render_texture_view,
            const std::shared_ptr<ITexture> &depth_texture,
            const std::shared_ptr<ITextureView> &depth_texture_view);

        void render(const std::shared_ptr<ICommandList> &command_list) const;

    private:
        const std::shared_ptr<ITexture> &m_render_texture;
        const std::shared_ptr<ITextureView> &m_render_texture_view;
        const std::shared_ptr<ITexture> &m_depth_texture;
        const std::shared_ptr<ITextureView> &m_depth_texture_view;

        std::shared_ptr<IPipelineLayout> m_pipeline_layout;
        std::shared_ptr<IShaderModule> m_vertex_shader;
        std::shared_ptr<IShaderModule> m_fragment_shader;
        std::shared_ptr<IRenderPipeline> m_pipeline;
    };
} // namespace hyper_engine