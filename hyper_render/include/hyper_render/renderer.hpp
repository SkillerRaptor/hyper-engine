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

namespace hyper_render
{
    struct RendererDescriptor
    {
        hyper_rhi::GraphicsDeviceHandle graphics_device;
        hyper_rhi::SurfaceHandle surface;
    };

    class Renderer
    {
    public:
        Renderer(hyper_event::EventBus &event_bus, const hyper_platform::Input &input, const RendererDescriptor &descriptor);

        // NOTE: This shouldn't be in the renderer
        void update(float delta_time);
        void render();

    private:
        void on_resize(const hyper_platform::WindowResizeEvent &event);
        void on_mouse_moved(const hyper_platform::MouseMovedEvent &event);
        void on_mouse_scolled(const hyper_platform::MouseScrolledEvent &event);

    private:
        const hyper_platform::Input &m_input;
        hyper_rhi::GraphicsDeviceHandle m_graphics_device;
        hyper_rhi::SurfaceHandle m_surface;
        hyper_rhi::ShaderCompiler m_shader_compiler;
        hyper_rhi::QueueHandle m_queue;
        hyper_rhi::CommandListHandle m_command_list;
        hyper_rhi::PipelineLayoutHandle m_pipeline_layout;
        hyper_rhi::ShaderModuleHandle m_vertex_shader;
        hyper_rhi::ShaderModuleHandle m_fragment_shader;
        hyper_rhi::GraphicsPipelineHandle m_pipeline;
        hyper_rhi::BufferHandle m_material_buffer;
        hyper_rhi::BufferHandle m_positions_buffer;
        hyper_rhi::BufferHandle m_normals_buffer;
        hyper_rhi::BufferHandle m_colors_buffer;
        hyper_rhi::BufferHandle m_mesh_buffer;
        hyper_rhi::BufferHandle m_indices_buffer;
        hyper_rhi::BufferHandle m_camera_buffer;

        Camera m_editor_camera;

        uint32_t m_frame_index;
    };
} // namespace hyper_render