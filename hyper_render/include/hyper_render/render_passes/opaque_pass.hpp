/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/nonnull_ref_ptr.hpp>
#include <hyper_rhi/forward.hpp>

namespace hyper_engine
{
    struct DrawContext;

    class OpaquePass
    {
    public:
        OpaquePass(
            const NonnullRefPtr<TextureView> &render_texture_view,
            const NonnullRefPtr<TextureView> &depth_texture_view,
            const NonnullRefPtr<Buffer> &scene_buffer);

        void render(const NonnullRefPtr<CommandList> &command_list, const DrawContext &draw_context) const;

    private:
        const NonnullRefPtr<TextureView> &m_render_texture_view;
        const NonnullRefPtr<TextureView> &m_depth_texture_view;
        const NonnullRefPtr<Buffer> &m_scene_buffer;
    };
} // namespace hyper_engine