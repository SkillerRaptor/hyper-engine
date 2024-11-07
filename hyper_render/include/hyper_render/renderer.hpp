/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_event/event_bus.hpp>
#include <hyper_platform/input.hpp>
#include <hyper_platform/mouse_events.hpp>
#include <hyper_platform/window_events.hpp>
#include <hyper_rhi/graphics_device.hpp>
#include <hyper_rhi/shader_compiler.hpp>
#include <hyper_rhi/surface.hpp>

#include "hyper_render/camera.hpp"
#include "hyper_render/mesh.hpp"
#include "hyper_render/render_passes/grid_pass.hpp"
#include "hyper_render/render_passes/opaque_pass.hpp"

namespace hyper_render
{
    struct RendererDescriptor
    {
        std::shared_ptr<hyper_rhi::GraphicsDevice> graphics_device;
        std::shared_ptr<hyper_rhi::Surface> surface;
    };

    class Renderer
    {
    public:
        Renderer(hyper_event::EventBus &event_bus, const hyper_platform::Input &input, const RendererDescriptor &descriptor);

        // NOTE: This shouldn't be in the renderer
        void update(float delta_time);
        void render();

    private:
        void create_textures(uint32_t width, uint32_t height);

        void on_resize(const hyper_platform::WindowResizeEvent &event);
        void on_mouse_move(const hyper_platform::MouseMovedEvent &event);
        void on_mouse_scroll(const hyper_platform::MouseScrolledEvent &event);

    private:
        const hyper_platform::Input &m_input;
        std::shared_ptr<hyper_rhi::GraphicsDevice> m_graphics_device;
        std::shared_ptr<hyper_rhi::Surface> m_surface;
        hyper_rhi::ShaderCompiler m_shader_compiler;
        std::shared_ptr<hyper_rhi::CommandList> m_command_list;

        std::shared_ptr<hyper_rhi::Texture> m_render_texture;
        std::shared_ptr<hyper_rhi::TextureView> m_render_texture_view;
        std::shared_ptr<hyper_rhi::Texture> m_depth_texture;
        std::shared_ptr<hyper_rhi::TextureView> m_depth_texture_view;

        Camera m_editor_camera;
        std::shared_ptr<hyper_rhi::Buffer> m_camera_buffer;

        std::unique_ptr<OpaquePass> m_opaque_pass;
        std::unique_ptr<GridPass> m_grid_pass;

        std::vector<std::shared_ptr<Mesh>> m_meshes;

        uint32_t m_frame_index;
    };
} // namespace hyper_render