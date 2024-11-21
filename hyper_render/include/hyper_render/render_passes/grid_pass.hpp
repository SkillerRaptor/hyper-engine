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
        class ICommandList;
        class IGraphicsDevice;
        class IPipelineLayout;
        class IRenderPipeline;
        class ShaderCompiler;
        class IShaderModule;
        class ITexture;
        class ITextureView;
    } // namespace rhi

    namespace render
    {
        class GridPass
        {
        public:
            GridPass(
                const std::shared_ptr<he::rhi::IGraphicsDevice> &graphics_device,
                const he::rhi::ShaderCompiler &shader_compiler,
                const std::shared_ptr<he::rhi::ITexture> &render_texture,
                const std::shared_ptr<he::rhi::ITextureView> &render_texture_view,
                const std::shared_ptr<he::rhi::ITexture> &depth_texture,
                const std::shared_ptr<he::rhi::ITextureView> &depth_texture_view);

            void render(const std::shared_ptr<he::rhi::ICommandList> &command_list) const;

        private:
            const std::shared_ptr<he::rhi::ITexture> &m_render_texture;
            const std::shared_ptr<he::rhi::ITextureView> &m_render_texture_view;
            const std::shared_ptr<he::rhi::ITexture> &m_depth_texture;
            const std::shared_ptr<he::rhi::ITextureView> &m_depth_texture_view;

            std::shared_ptr<he::rhi::IPipelineLayout> m_pipeline_layout;
            std::shared_ptr<he::rhi::IShaderModule> m_vertex_shader;
            std::shared_ptr<he::rhi::IShaderModule> m_fragment_shader;
            std::shared_ptr<he::rhi::IRenderPipeline> m_pipeline;
        };
    } // namespace render
} // namespace he