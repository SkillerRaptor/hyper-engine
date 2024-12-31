/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

namespace hyper_engine
{
    class Buffer;
    struct BufferDescriptor;
    class CommandList;
    struct ComputePipelineDescriptor;
    class ComputePipeline;
    class ImGuiManager;
    struct PipelineLayoutDescriptor;
    class PipelineLayout;
    struct RenderPipelineDescriptor;
    class RenderPipeline;
    struct SamplerDescriptor;
    class Sampler;
    struct ShaderModuleDescriptor;
    class ShaderModule;
    class Surface;
    struct TextureDescriptor;
    class Texture;
    struct TextureViewDescriptor;
    class TextureView;
    class Window;

    enum class GraphicsApi
    {
        D3D12,
        Vulkan,
    };

    struct GraphicsDeviceDescriptor
    {
        GraphicsApi graphics_api = GraphicsApi::Vulkan;
        bool debug_validation = false;
        bool debug_label = false;
        bool debug_marker = false;
    };

    class GraphicsDevice
    {
    public:
        static constexpr size_t s_frame_count = 2;
        static constexpr size_t s_descriptor_limit = 1000 * 1000;

    public:
        virtual ~GraphicsDevice() = default;

        static GraphicsDevice *create(const GraphicsDeviceDescriptor &descriptor);

        virtual std::shared_ptr<Surface> create_surface() = 0;
        virtual std::shared_ptr<Buffer> create_buffer(const BufferDescriptor &descriptor) = 0;
        virtual std::shared_ptr<CommandList> create_command_list() = 0;
        virtual std::shared_ptr<ComputePipeline> create_compute_pipeline(const ComputePipelineDescriptor &descriptor) = 0;
        virtual std::shared_ptr<RenderPipeline> create_render_pipeline(const RenderPipelineDescriptor &descriptor) = 0;
        virtual std::shared_ptr<PipelineLayout> create_pipeline_layout(const PipelineLayoutDescriptor &descriptor) = 0;
        virtual std::shared_ptr<Sampler> create_sampler(const SamplerDescriptor &descriptor) = 0;
        virtual std::shared_ptr<ShaderModule> create_shader_module(const ShaderModuleDescriptor &descriptor) = 0;
        virtual std::shared_ptr<Texture> create_texture(const TextureDescriptor &descriptor) = 0;
        virtual std::shared_ptr<TextureView> create_texture_view(const TextureViewDescriptor &descriptor) = 0;

        virtual std::shared_ptr<ImGuiManager> create_imgui_manager() = 0;

        virtual void begin_frame(const std::shared_ptr<Surface> &surface, uint32_t frame_index) = 0;
        virtual void end_frame() const = 0;
        virtual void execute(const std::shared_ptr<CommandList> &command_list) = 0;
        virtual void present(const std::shared_ptr<Surface> &surface) const = 0;

        virtual void wait_for_idle() const = 0;

        virtual GraphicsApi graphics_api() const = 0;
        virtual bool debug_validation() const = 0;
        virtual bool debug_label() const = 0;
        virtual bool debug_marker() const = 0;

    protected:
    };
} // namespace hyper_engine
