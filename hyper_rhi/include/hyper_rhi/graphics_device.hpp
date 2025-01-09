/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/nonnull_ref_ptr.hpp>
#include <hyper_core/nonnull_own_ptr.hpp>

#include "hyper_rhi/forward.hpp"
#include "hyper_rhi/resource_handle.hpp"

namespace hyper_engine
{
    // NOTE: Don't add this to forward.hpp, as it will be only used here
    class DescriptorManager;

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

        virtual NonnullRefPtr<Surface> create_surface() = 0;
        // FIXME: Make this creatable for every frame for the current command buffer
        virtual NonnullRefPtr<CommandList> create_command_list() = 0;

        NonnullRefPtr<Buffer> create_buffer(const BufferDescriptor &descriptor);
        NonnullRefPtr<Buffer> create_buffer(const BufferDescriptor &descriptor, ResourceHandle handle);

        NonnullRefPtr<ComputePipeline> create_compute_pipeline(const ComputePipelineDescriptor &descriptor) const;
        NonnullRefPtr<RenderPipeline> create_render_pipeline(const RenderPipelineDescriptor &descriptor) const;
        NonnullRefPtr<PipelineLayout> create_pipeline_layout(const PipelineLayoutDescriptor &descriptor) const;
        NonnullRefPtr<ShaderModule> create_shader_module(const ShaderModuleDescriptor &descriptor) const;

        NonnullRefPtr<Sampler> create_sampler(const SamplerDescriptor &descriptor);
        NonnullRefPtr<Sampler> create_sampler(const SamplerDescriptor &descriptor, ResourceHandle handle);
        NonnullRefPtr<Texture> create_texture(const TextureDescriptor &descriptor) const;
        NonnullRefPtr<TextureView> create_texture_view(const TextureViewDescriptor &descriptor);
        NonnullRefPtr<TextureView> create_texture_view(const TextureViewDescriptor &descriptor, ResourceHandle handle);

        virtual void begin_frame(Surface &surface, uint32_t frame_index) = 0;
        virtual void end_frame() const = 0;
        virtual void execute(const CommandList &command_list) = 0;
        virtual void present(const Surface &surface) const = 0;

        virtual void wait_for_idle() const = 0;

        virtual GraphicsApi graphics_api() const = 0;
        virtual bool debug_validation() const = 0;
        virtual bool debug_label() const = 0;
        virtual bool debug_marker() const = 0;

    protected:
        virtual NonnullRefPtr<Buffer> create_buffer_platform(const BufferDescriptor &descriptor, ResourceHandle handle) const = 0;

        virtual NonnullRefPtr<ComputePipeline> create_compute_pipeline_platform(const ComputePipelineDescriptor &descriptor) const = 0;
        virtual NonnullRefPtr<RenderPipeline> create_render_pipeline_platform(const RenderPipelineDescriptor &descriptor) const = 0;
        virtual NonnullRefPtr<PipelineLayout> create_pipeline_layout_platform(const PipelineLayoutDescriptor &descriptor) const = 0;
        virtual NonnullRefPtr<ShaderModule> create_shader_module_platform(const ShaderModuleDescriptor &descriptor) const = 0;

        virtual NonnullRefPtr<Sampler> create_sampler_platform(const SamplerDescriptor &descriptor, ResourceHandle handle) const = 0;
        virtual NonnullRefPtr<Texture> create_texture_platform(const TextureDescriptor &descriptor) const = 0;
        virtual NonnullRefPtr<TextureView>
            create_texture_view_platform(const TextureViewDescriptor &descriptor, ResourceHandle handle) const = 0;

    protected:
        virtual DescriptorManager &descriptor_manager() = 0;
    };
} // namespace hyper_engine
