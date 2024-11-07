/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <D3D12MemAlloc.h>

#include "hyper_rhi/graphics_device.hpp"
#include "hyper_rhi/d3d12/d3d12_common.hpp"

namespace hyper_rhi
{
    class D3D12GraphicsDevice final : public GraphicsDevice
    {
    public:
        explicit D3D12GraphicsDevice(const GraphicsDeviceDescriptor &descriptor);

        std::shared_ptr<Surface> create_surface(const hyper_platform::Window &window) override;
        std::shared_ptr<Buffer> create_buffer(const BufferDescriptor &descriptor) override;
        std::shared_ptr<CommandList> create_command_list() override;
        std::shared_ptr<ComputePipeline> create_compute_pipeline(const ComputePipelineDescriptor &descriptor) override;
        std::shared_ptr<RenderPipeline> create_render_pipeline(const RenderPipelineDescriptor &descriptor) override;
        std::shared_ptr<PipelineLayout> create_pipeline_layout(const PipelineLayoutDescriptor &descriptor) override;
        std::shared_ptr<Sampler> create_sampler(const SamplerDescriptor &descriptor) override;
        std::shared_ptr<ShaderModule> create_shader_module(const ShaderModuleDescriptor &descriptor) override;
        std::shared_ptr<Texture> create_texture(const TextureDescriptor &descriptor) override;
        std::shared_ptr<TextureView> create_texture_view(const TextureViewDescriptor &descriptor) override;

        void begin_frame(const std::shared_ptr<Surface> &surface, uint32_t frame_index) override;
        void end_frame() const override;
        void execute(const std::shared_ptr<CommandList> &command_list) const override;
        void present(const std::shared_ptr<Surface> &surface) const override;

        void wait_for_idle() const override;

        [[nodiscard]] ComPtr<IDXGIFactory7> factory() const;
        [[nodiscard]] ComPtr<ID3D12CommandQueue> command_queue() const;

    private:
        void enable_debug_layers();
        void create_factory();
        void choose_adapter();
        void create_device();
        void create_command_queue();
        void create_allocator();

    private:
        ComPtr<IDXGIFactory7> m_factory;
        ComPtr<IDXGIAdapter4> m_adapter;
        ComPtr<ID3D12Device> m_device;
        ComPtr<ID3D12CommandQueue> m_command_queue;
        ComPtr<D3D12MA::Allocator> m_allocator;
    };
} // namespace hyper_rhi
