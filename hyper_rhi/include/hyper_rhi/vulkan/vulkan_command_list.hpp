/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/command_list.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_engine
{
    class VulkanGraphicsDevice;

    class VulkanCommandList final : public CommandList
    {
    public:
        void begin() override;
        void end() override;

        void insert_barriers(const Barriers &barriers) const override;

        void clear_buffer(const Buffer &buffer, size_t size, uint64_t offset) override;
        void clear_texture(const Texture &texture, SubresourceRange subresource_range) override;

        void copy_buffer_to_buffer(const Buffer &src, uint64_t src_offset, const Buffer &dst, uint64_t dst_offset, size_t size) override;
        void copy_buffer_to_texture(
            const Buffer &src,
            uint64_t src_offset,
            const Texture &dst,
            Offset3d dst_offset,
            Extent3d dst_extent,
            uint32_t dst_mip_level,
            uint32_t dst_array_index) override;
        void copy_texture_to_buffer(
            const Texture &src,
            Offset3d src_offset,
            Extent3d src_extent,
            uint32_t src_mip_level,
            uint32_t src_array_index,
            const Buffer &dst,
            uint64_t dst_offset) override;
        void copy_texture_to_texture(
            const Texture &src,
            Offset3d src_offset,
            uint32_t src_mip_level,
            uint32_t src_array_index,
            const Texture &dst,
            Offset3d dst_offset,
            uint32_t dst_mip_level,
            uint32_t dst_array_index,
            Extent3d extent) override;

        void write_buffer(const Buffer &buffer, const void *data, size_t size, uint64_t offset) override;
        void write_texture(
            const Texture &texture,
            Offset3d offset,
            Extent3d extent,
            uint32_t mip_level,
            uint32_t array_index,
            const void *data,
            size_t data_size,
            uint64_t data_offset) override;

        RefPtr<ComputePass> begin_compute_pass_platform(const ComputePassDescriptor &descriptor) const override;
        RefPtr<RenderPass> begin_render_pass_platform(const RenderPassDescriptor &descriptor) const override;

        VkCommandBuffer command_buffer() const;

        static VkPipelineStageFlags2 get_pipeline_stage_flags(BitFlags<BarrierPipelineStage> barrier_pipeline_stage);
        static VkAccessFlags2 get_access_flags(BitFlags<BarrierAccess> barrier_access);
        static VkImageLayout get_image_layout(BarrierTextureLayout texture_layout);

    private:
        VkCommandBuffer m_command_buffer = VK_NULL_HANDLE;
    };
} // namespace hyper_engine