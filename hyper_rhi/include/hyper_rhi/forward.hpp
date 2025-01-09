/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

namespace hyper_engine
{
    struct BufferDescriptor;
    class Buffer;

    class CommandList;

    struct ComputePassDescriptor;
    class ComputePass;

    struct ComputePipelineDescriptor;
    class ComputePipeline;

    struct GraphicsDeviceDescriptor;
    class GraphicsDevice;

    struct PipelineLayoutDescriptor;
    class PipelineLayout;

    struct RenderPassDescriptor;
    class RenderPass;

    struct RenderPipelineDescriptor;
    class RenderPipeline;

    struct SamplerDescriptor;
    class Sampler;

    struct ShaderModuleDescriptor;
    class ShaderModule;

    class ShaderCompiler;

    class Surface;

    struct TextureDescriptor;
    class Texture;

    struct TextureViewDescriptor;
    class TextureView;

    // NOTE: This should be removed
    class ImGuiManager;
} // namespace hyper_engine