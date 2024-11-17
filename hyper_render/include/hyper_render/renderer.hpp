/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <unordered_map>

#include <hyper_rhi/shader_compiler.hpp>

#include "hyper_render/camera.hpp"
#include "hyper_render/renderable.hpp"

namespace he
{
    namespace event
    {
        class EventBus;
    } // namespace event

    namespace platform
    {
        class MouseMoveEvent;
        class MouseScrollEvent;
        class Window;
        class WindowResizeEvent;
    } // namespace platform

    namespace rhi
    {
        class Surface;
    } // namespace rhi

    namespace render
    {
        class GridPass;
        class ImGuiPass;
        class OpaquePass;

        struct RendererDescriptor
        {
            std::shared_ptr<he::rhi::GraphicsDevice> graphics_device;
            std::shared_ptr<he::rhi::Surface> surface;
        };

        class Renderer
        {
        public:
            Renderer(he::event::EventBus &event_bus, const he::platform::Window &window, const RendererDescriptor &descriptor);
            ~Renderer();

            // NOTE: This shouldn't be in the renderer
            void update(float delta_time);
            void render();

        private:
            void create_textures(uint32_t width, uint32_t height);

            void update_scene();

            void on_resize(const he::platform::WindowResizeEvent &event);
            void on_mouse_move(const he::platform::MouseMoveEvent &event);
            void on_mouse_scroll(const he::platform::MouseScrollEvent &event);

        private:
            std::shared_ptr<he::rhi::GraphicsDevice> m_graphics_device;
            std::shared_ptr<he::rhi::Surface> m_surface;
            he::rhi::ShaderCompiler m_shader_compiler;
            std::shared_ptr<he::rhi::CommandList> m_command_list;

            std::shared_ptr<he::rhi::Texture> m_render_texture;
            std::shared_ptr<he::rhi::TextureView> m_render_texture_view;
            std::shared_ptr<he::rhi::Texture> m_depth_texture;
            std::shared_ptr<he::rhi::TextureView> m_depth_texture_view;

            Camera m_editor_camera;
            std::shared_ptr<he::rhi::Buffer> m_camera_buffer;

            std::shared_ptr<he::rhi::Buffer> m_scene_buffer;

            std::shared_ptr<he::rhi::Texture> m_white_texture;
            std::shared_ptr<he::rhi::TextureView> m_white_texture_view;
            std::shared_ptr<he::rhi::Texture> m_error_texture;
            std::shared_ptr<he::rhi::TextureView> m_error_texture_view;
            std::shared_ptr<he::rhi::Sampler> m_default_sampler_nearest;
            std::shared_ptr<he::rhi::Sampler> m_default_sampler_linear;

            GltfMetallicRoughness m_metallic_roughness_material;

            DrawContext m_draw_context;
            std::unordered_map<std::string, std::shared_ptr<LoadedGltf>> m_scenes;

            std::unique_ptr<OpaquePass> m_opaque_pass;
            std::unique_ptr<GridPass> m_grid_pass;
            std::unique_ptr<ImGuiPass> m_imgui_pass;

            uint32_t m_frame_index;
        };
    } // namespace render
} // namespace he