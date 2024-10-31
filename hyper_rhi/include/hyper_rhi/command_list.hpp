/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

// NOTE: Based on the implementation of rtryan98: https://github.com/rtryan98/rhi

#pragma once

#include <memory>

#include <hyper_core/bitmask.hpp>

#include "hyper_rhi/buffer.hpp"
#include "hyper_rhi/render_pass.hpp"
#include "hyper_rhi/texture.hpp"

namespace hyper_rhi
{
    enum class BarrierPipelineStage : uint16_t
    {
        None = 0,
        ComputeShader = 1 << 0,
        VertexShader = 1 << 1,
        FragmentShader = 1 << 2,
        EarlyFragmentTests = 1 << 3,
        LateFragmentTests = 1 << 4,
        ColorAttachmentOutput = 1 << 5,
        Copy = 1 << 6,
        Clear = 1 << 7,
        AllGraphics = 1 << 8,
        AllTransfer = 1 << 9,
        AllCommands = 1 << 10,
    };

    HE_ENABLE_BITMASK_OPERATORS(BarrierPipelineStage);

    enum class BarrierAccess : uint8_t
    {
        None = 0,
        ShaderRead = 1 << 0,
        ShaderWrite = 1 << 1,
        ColorAttachmentRead = 1 << 2,
        ColorAttachmentWrite = 1 << 3,
        DepthStencilAttachmentRead = 1 << 4,
        DepthStencilAttachmentWrite = 1 << 5,
        TransferRead = 1 << 6,
        TransferWrite = 1 << 7,
    };

    HE_ENABLE_BITMASK_OPERATORS(BarrierAccess);

    struct MemoryBarrier
    {
        BarrierPipelineStage stage_before = BarrierPipelineStage::None;
        BarrierPipelineStage stage_after = BarrierPipelineStage::None;
        BarrierAccess access_before = BarrierAccess::None;
        BarrierAccess access_after = BarrierAccess::None;
    };

    struct BufferMemoryBarrier
    {
        BarrierPipelineStage stage_before = BarrierPipelineStage::None;
        BarrierPipelineStage stage_after = BarrierPipelineStage::None;
        BarrierAccess access_before = BarrierAccess::None;
        BarrierAccess access_after = BarrierAccess::None;
        std::shared_ptr<Buffer> &buffer;
    };

    enum class BarrierTextureLayout
    {
        Undefined,
        General,
        Present,
        ColorAttachment,
        DepthStencilAttachment,
    };

    struct TextureBarrierSubresourceRange
    {
        uint32_t base_mip_level = 0;
        uint32_t mip_level_count = 1;
        uint32_t base_array_level = 0;
        uint32_t array_layer_count = 1;
    };

    struct TextureMemoryBarrier
    {
        BarrierPipelineStage stage_before = BarrierPipelineStage::None;
        BarrierPipelineStage stage_after = BarrierPipelineStage::None;
        BarrierAccess access_before = BarrierAccess::None;
        BarrierAccess access_after = BarrierAccess::None;
        BarrierTextureLayout layout_before = BarrierTextureLayout::Undefined;
        BarrierTextureLayout layout_after = BarrierTextureLayout::Undefined;
        std::shared_ptr<Texture> texture;
        TextureBarrierSubresourceRange subresource_range;
    };

    struct Barriers
    {
        std::vector<MemoryBarrier> memory_barriers;
        std::vector<BufferMemoryBarrier> buffer_memory_barriers;
        std::vector<TextureMemoryBarrier> texture_memory_barriers;
    };

    class CommandList
    {
    public:
        virtual ~CommandList() = default;

        virtual void begin() = 0;
        virtual void end() = 0;

        virtual void insert_barriers(const Barriers &barriers) const = 0;

        [[nodiscard]] virtual std::shared_ptr<RenderPass> begin_render_pass(const RenderPassDescriptor &descriptor) const = 0;
    };
} // namespace hyper_rhi
