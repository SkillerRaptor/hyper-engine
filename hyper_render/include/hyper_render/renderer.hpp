/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <unordered_map>

#include <hyper_core/own_ptr.hpp>
#include <hyper_platform/forward.hpp>
#include <hyper_rhi/forward.hpp>
#include <hyper_rhi/graphics_device.hpp>
#include <hyper_rhi/shader_compiler.hpp>

#include "hyper_render/camera.hpp"
#include "hyper_render/renderable.hpp"

namespace hyper_engine
{
    class EventBus;
    class GridPass;
    class OpaquePass;

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        // NOTE: This shouldn't be in the renderer
        void update(float delta_time);
        void render();

    private:
        void create_textures(uint32_t width, uint32_t height);

        void update_scene();

        void on_resize(const WindowResizeEvent &event);
        void on_mouse_move(const MouseMoveEvent &event);
        void on_mouse_scroll(const MouseScrollEvent &event);

    private:
        RefPtr<Surface> m_surface;
        ShaderCompiler m_shader_compiler;
        RefPtr<CommandList> m_command_list;

        RefPtr<Texture> m_render_texture;
        RefPtr<TextureView> m_render_texture_view;
        RefPtr<Texture> m_depth_texture;
        RefPtr<TextureView> m_depth_texture_view;

        Camera m_editor_camera;
        RefPtr<Buffer> m_camera_buffer;

        RefPtr<Buffer> m_scene_buffer;

        RefPtr<Texture> m_white_texture;
        RefPtr<TextureView> m_white_texture_view;
        RefPtr<Texture> m_error_texture;
        RefPtr<TextureView> m_error_texture_view;
        RefPtr<Sampler> m_default_sampler_nearest;
        RefPtr<Sampler> m_default_sampler_linear;

        GltfMetallicRoughness m_metallic_roughness_material;

        DrawContext m_draw_context;
        std::unordered_map<std::string, RefPtr<LoadedGltf>> m_scenes;

        OwnPtr<OpaquePass> m_opaque_pass;
        OwnPtr<GridPass> m_grid_pass;

        uint32_t m_frame_index = 1;
    };
} // namespace hyper_engine