/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <unordered_map>

#include <hyper_rhi/graphics_device.hpp>
#include <hyper_rhi/shader_compiler.hpp>

#include "hyper_render/camera.hpp"
#include "hyper_render/renderable.hpp"

namespace hyper_engine
{
    class EventBus;
    class GridPass;
    class ImGuiPass;
    class Surface;
    class MouseMoveEvent;
    class MouseScrollEvent;
    class OpaquePass;
    class Window;
    class WindowResizeEvent;

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
        std::shared_ptr<Surface> m_surface;
        ShaderCompiler m_shader_compiler;
        std::shared_ptr<CommandList> m_command_list;

        std::shared_ptr<Texture> m_render_texture;
        std::shared_ptr<TextureView> m_render_texture_view;
        std::shared_ptr<Texture> m_depth_texture;
        std::shared_ptr<TextureView> m_depth_texture_view;

        Camera m_editor_camera;
        std::shared_ptr<Buffer> m_camera_buffer;

        std::shared_ptr<Buffer> m_scene_buffer;

        std::shared_ptr<Texture> m_white_texture;
        std::shared_ptr<TextureView> m_white_texture_view;
        std::shared_ptr<Texture> m_error_texture;
        std::shared_ptr<TextureView> m_error_texture_view;
        std::shared_ptr<Sampler> m_default_sampler_nearest;
        std::shared_ptr<Sampler> m_default_sampler_linear;

        GltfMetallicRoughness m_metallic_roughness_material;

        DrawContext m_draw_context;
        std::unordered_map<std::string, std::shared_ptr<LoadedGltf>> m_scenes;

        std::unique_ptr<OpaquePass> m_opaque_pass;
        std::unique_ptr<GridPass> m_grid_pass;
        std::unique_ptr<ImGuiPass> m_imgui_pass;

        uint32_t m_frame_index;
    };
} // namespace hyper_engine