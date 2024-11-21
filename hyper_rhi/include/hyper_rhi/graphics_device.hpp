/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

namespace hyper_engine
{
    class IBuffer;
    struct BufferDescriptor;
    class ICommandList;
    struct ComputePipelineDescriptor;
    class IComputePipeline;
    class IImGuiManager;
    struct PipelineLayoutDescriptor;
    class IPipelineLayout;
    struct RenderPipelineDescriptor;
    class IRenderPipeline;
    struct SamplerDescriptor;
    class ISampler;
    struct ShaderModuleDescriptor;
    class IShaderModule;
    class ISurface;
    struct TextureDescriptor;
    class ITexture;
    struct TextureViewDescriptor;
    class ITextureView;
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

    class IGraphicsDevice
    {
    public:
        static constexpr size_t s_frame_count = 2;
        static constexpr size_t s_descriptor_limit = 1000 * 1000;

    public:
        explicit IGraphicsDevice(const GraphicsDeviceDescriptor &descriptor);
        virtual ~IGraphicsDevice() = default;

        static std::shared_ptr<IGraphicsDevice> create(const GraphicsDeviceDescriptor &descriptor);

        virtual std::shared_ptr<ISurface> create_surface(const Window &window) = 0;
        virtual std::shared_ptr<IBuffer> create_buffer(const BufferDescriptor &descriptor) = 0;
        virtual std::shared_ptr<ICommandList> create_command_list() = 0;
        virtual std::shared_ptr<IComputePipeline> create_compute_pipeline(const ComputePipelineDescriptor &descriptor) = 0;
        virtual std::shared_ptr<IRenderPipeline> create_render_pipeline(const RenderPipelineDescriptor &descriptor) = 0;
        virtual std::shared_ptr<IPipelineLayout> create_pipeline_layout(const PipelineLayoutDescriptor &descriptor) = 0;
        virtual std::shared_ptr<ISampler> create_sampler(const SamplerDescriptor &descriptor) = 0;
        virtual std::shared_ptr<IShaderModule> create_shader_module(const ShaderModuleDescriptor &descriptor) = 0;
        virtual std::shared_ptr<ITexture> create_texture(const TextureDescriptor &descriptor) = 0;
        virtual std::shared_ptr<ITextureView> create_texture_view(const TextureViewDescriptor &descriptor) = 0;

        virtual std::shared_ptr<IImGuiManager> create_imgui_manager() = 0;

        virtual void begin_frame(const std::shared_ptr<ISurface> &surface, uint32_t frame_index) = 0;
        virtual void end_frame() const = 0;
        virtual void execute(const std::shared_ptr<ICommandList> &command_list) = 0;
        virtual void present(const std::shared_ptr<ISurface> &surface) const = 0;

        virtual void wait_for_idle() const = 0;

        GraphicsApi graphics_api() const;
        bool debug_validation() const;
        bool debug_label() const;
        bool debug_marker() const;

    protected:
        GraphicsApi m_graphics_api;
        bool m_debug_validation;
        bool m_debug_label;
        bool m_debug_marker;
    };
} // namespace hyper_engine
