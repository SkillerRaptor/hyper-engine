/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/nonnull_ref_ptr.hpp>
#include <hyper_core/ref_ptr.hpp>
#include <hyper_rhi/forward.hpp>
#include <hyper_rhi/shader_module.hpp>
#include <hyper_rhi/render_pipeline.hpp>
#include <hyper_rhi/pipeline_layout.hpp>

namespace hyper_engine
{
    class GridPass
    {
    public:
        GridPass(
            const ShaderCompiler &shader_compiler,
            const NonnullRefPtr<Texture> &render_texture,
            const NonnullRefPtr<TextureView> &render_texture_view,
            const NonnullRefPtr<Texture> &depth_texture,
            const NonnullRefPtr<TextureView> &depth_texture_view);

        void render(const NonnullRefPtr<CommandList> &command_list) const;

    private:
        const NonnullRefPtr<Texture> &m_render_texture;
        const NonnullRefPtr<TextureView> &m_render_texture_view;
        const NonnullRefPtr<Texture> &m_depth_texture;
        const NonnullRefPtr<TextureView> &m_depth_texture_view;

        NonnullRefPtr<PipelineLayout> m_pipeline_layout;
        NonnullRefPtr<ShaderModule> m_vertex_shader;
        NonnullRefPtr<ShaderModule> m_fragment_shader;
        // FIXME: This should be a NonnullRefPtr
        RefPtr<RenderPipeline> m_pipeline;
    };
} // namespace hyper_engine