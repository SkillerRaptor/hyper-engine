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
        class Buffer;
        class CommandList;
        class TextureView;
    } // namespace rhi

    namespace render
    {
        struct DrawContext;

        class OpaquePass
        {
        public:
            OpaquePass(
                const std::shared_ptr<he::rhi::TextureView> &render_texture_view,
                const std::shared_ptr<he::rhi::TextureView> &depth_texture_view,
                const std::shared_ptr<he::rhi::Buffer> &scene_buffer);

            void render(const std::shared_ptr<he::rhi::CommandList> &command_list, const DrawContext &draw_context) const;

        private:
            const std::shared_ptr<he::rhi::TextureView> &m_render_texture_view;
            const std::shared_ptr<he::rhi::TextureView> &m_depth_texture_view;
            const std::shared_ptr<he::rhi::Buffer> &m_scene_buffer;
        };
    } // namespace render
} // namespace he