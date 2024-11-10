/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

#include "hyper_rhi/buffer.hpp"
#include "hyper_rhi/command_list.hpp"
#include "hyper_rhi/compute_pipeline.hpp"
#include "hyper_rhi/imgui_manager.hpp"
#include "hyper_rhi/pipeline_layout.hpp"
#include "hyper_rhi/render_pipeline.hpp"
#include "hyper_rhi/shader_module.hpp"
#include "hyper_rhi/surface.hpp"
#include "hyper_rhi/sampler.hpp"
#include "hyper_rhi/texture.hpp"
#include "hyper_rhi/texture_view.hpp"

namespace hyper_rhi
{
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
        explicit GraphicsDevice(const GraphicsDeviceDescriptor &descriptor)
            : m_graphics_api(descriptor.graphics_api)
            , m_debug_validation(descriptor.debug_validation)
            , m_debug_label(descriptor.debug_label)
            , m_debug_marker(descriptor.debug_marker)
        {
        }

        virtual ~GraphicsDevice() = default;

        static std::shared_ptr<GraphicsDevice> create(const GraphicsDeviceDescriptor &descriptor);

        [[nodiscard]] virtual std::shared_ptr<Surface> create_surface(const hyper_platform::Window &window) = 0;
        [[nodiscard]] virtual std::shared_ptr<Buffer> create_buffer(const BufferDescriptor &descriptor) = 0;
        [[nodiscard]] virtual std::shared_ptr<CommandList> create_command_list() = 0;
        [[nodiscard]] virtual std::shared_ptr<ComputePipeline> create_compute_pipeline(const ComputePipelineDescriptor &descriptor) = 0;
        [[nodiscard]] virtual std::shared_ptr<RenderPipeline> create_render_pipeline(const RenderPipelineDescriptor &descriptor) = 0;
        [[nodiscard]] virtual std::shared_ptr<PipelineLayout> create_pipeline_layout(const PipelineLayoutDescriptor &descriptor) = 0;
        [[nodiscard]] virtual std::shared_ptr<Sampler> create_sampler(const SamplerDescriptor &descriptor) = 0;
        [[nodiscard]] virtual std::shared_ptr<ShaderModule> create_shader_module(const ShaderModuleDescriptor &descriptor) = 0;
        [[nodiscard]] virtual std::shared_ptr<Texture> create_texture(const TextureDescriptor &descriptor) = 0;
        [[nodiscard]] virtual std::shared_ptr<TextureView> create_texture_view(const TextureViewDescriptor &descriptor) = 0;

        [[nodiscard]] virtual std::shared_ptr<ImGuiManager> create_imgui_manager() = 0;

        virtual void begin_frame(const std::shared_ptr<Surface> &surface, uint32_t frame_index) = 0;
        virtual void end_frame() const = 0;
        virtual void execute(const std::shared_ptr<CommandList> &command_list) = 0;
        virtual void present(const std::shared_ptr<Surface> &surface) const = 0;

        virtual void wait_for_idle() const = 0;

        [[nodiscard]] HE_FORCE_INLINE GraphicsApi graphics_api() const
        {
            return m_graphics_api;
        }

        [[nodiscard]] HE_FORCE_INLINE bool debug_validation() const
        {
            return m_debug_validation;
        }

        [[nodiscard]] HE_FORCE_INLINE bool debug_label() const
        {
            return m_debug_label;
        }

        [[nodiscard]] HE_FORCE_INLINE bool debug_marker() const
        {
            return m_debug_marker;
        }

    protected:
        GraphicsApi m_graphics_api;
        bool m_debug_validation;
        bool m_debug_label;
        bool m_debug_marker;
    };
} // namespace hyper_rhi
