/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

namespace hyper_engine
{
    struct DrawContext;
    class IBuffer;
    class ICommandList;
    class ITextureView;

    class OpaquePass
    {
    public:
        OpaquePass(
            const std::shared_ptr<ITextureView> &render_texture_view,
            const std::shared_ptr<ITextureView> &depth_texture_view,
            const std::shared_ptr<IBuffer> &scene_buffer);

        void render(const std::shared_ptr<ICommandList> &command_list, const DrawContext &draw_context) const;

    private:
        const std::shared_ptr<ITextureView> &m_render_texture_view;
        const std::shared_ptr<ITextureView> &m_depth_texture_view;
        const std::shared_ptr<IBuffer> &m_scene_buffer;
    };
} // namespace hyper_engine