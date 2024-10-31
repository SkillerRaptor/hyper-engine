/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <optional>

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
    struct GeometrySurface
    {
        uint32_t start_index;
        uint32_t count;
    };

    struct MeshAsset
    {
        std::string name;
        std::vector<GeometrySurface> surfaces;
        std::shared_ptr<hyper_rhi::Buffer> positions;
        std::shared_ptr<hyper_rhi::Buffer> normals;
        std::shared_ptr<hyper_rhi::Buffer> colors;
        std::shared_ptr<hyper_rhi::Buffer> mesh;
        std::shared_ptr<hyper_rhi::Buffer> indices;
    };

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

        [[nodiscard]] std::optional<std::vector<std::shared_ptr<MeshAsset>>> load_model(const std::string &path) const;

    private:
        void on_resize(const hyper_platform::WindowResizeEvent &event);
        void on_mouse_moved(const hyper_platform::MouseMovedEvent &event);
        void on_mouse_scolled(const hyper_platform::MouseScrolledEvent &event);

    private:
        const hyper_platform::Input &m_input;
        std::shared_ptr<hyper_rhi::GraphicsDevice> m_graphics_device;
        std::shared_ptr<hyper_rhi::Surface> m_surface;
        std::shared_ptr<hyper_rhi::Texture> m_depth_texture;
        hyper_rhi::ShaderCompiler m_shader_compiler;
        std::shared_ptr<hyper_rhi::Queue> m_queue;
        std::shared_ptr<hyper_rhi::CommandList> m_command_list;

        std::shared_ptr<hyper_rhi::PipelineLayout> m_pipeline_layout;
        std::shared_ptr<hyper_rhi::ShaderModule> m_vertex_shader;
        std::shared_ptr<hyper_rhi::ShaderModule> m_fragment_shader;
        std::shared_ptr<hyper_rhi::GraphicsPipeline> m_pipeline;

        std::shared_ptr<hyper_rhi::Buffer> m_material_buffer;
        std::shared_ptr<hyper_rhi::Buffer> m_positions_buffer;
        std::shared_ptr<hyper_rhi::Buffer> m_normals_buffer;
        std::shared_ptr<hyper_rhi::Buffer> m_colors_buffer;
        std::shared_ptr<hyper_rhi::Buffer> m_mesh_buffer;
        std::shared_ptr<hyper_rhi::Buffer> m_indices_buffer;
        std::shared_ptr<hyper_rhi::Buffer> m_camera_buffer;

        std::shared_ptr<hyper_rhi::PipelineLayout> m_grid_pipeline_layout;
        std::shared_ptr<hyper_rhi::ShaderModule> m_grid_vertex_shader;
        std::shared_ptr<hyper_rhi::ShaderModule> m_grid_fragment_shader;
        std::shared_ptr<hyper_rhi::GraphicsPipeline> m_grid_pipeline;

        std::vector<std::shared_ptr<MeshAsset>> m_meshes;

        Camera m_editor_camera;

        uint32_t m_frame_index;
    };
} // namespace hyper_render