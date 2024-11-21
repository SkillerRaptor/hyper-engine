/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

#include "hyper_render/mesh.hpp"

namespace he
{
    namespace rhi
    {
        class IBuffer;
        class ICommandList;
        class ITextureView;
    } // namespace rhi

    namespace render
    {
        struct DrawContext;

        class OpaquePass
        {
        public:
            OpaquePass(
                const std::shared_ptr<he::rhi::ITextureView> &render_texture_view,
                const std::shared_ptr<he::rhi::ITextureView> &depth_texture_view,
                const std::shared_ptr<he::rhi::IBuffer> &scene_buffer);

            void render(const std::shared_ptr<he::rhi::ICommandList> &command_list, const DrawContext &draw_context) const;

        private:
            const std::shared_ptr<he::rhi::ITextureView> &m_render_texture_view;
            const std::shared_ptr<he::rhi::ITextureView> &m_depth_texture_view;
            const std::shared_ptr<he::rhi::IBuffer> &m_scene_buffer;
        };
    } // namespace render
} // namespace he