/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/command_list.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_engine
{
    class VulkanGraphicsDevice;

    class VulkanCommandList final : public ICommandList
    {
    public:
        explicit VulkanCommandList(VulkanGraphicsDevice &graphics_device);

        void begin() override;
        void end() override;

        void insert_barriers(const Barriers &barriers) const override;

        void clear_buffer(const std::shared_ptr<IBuffer> &buffer, size_t size, uint64_t offset) override;
        void clear_texture(const std::shared_ptr<ITexture> &texture, SubresourceRange subresource_range) override;

        void copy_buffer_to_buffer(
            const std::shared_ptr<IBuffer> &src,
            uint64_t src_offset,
            const std::shared_ptr<IBuffer> &dst,
            uint64_t dst_offset,
            size_t size) override;
        void copy_buffer_to_texture(
            const std::shared_ptr<IBuffer> &src,
            uint64_t src_offset,
            const std::shared_ptr<ITexture> &dst,
            Offset3d dst_offset,
            Extent3d dst_extent,
            uint32_t dst_mip_level,
            uint32_t dst_array_index) override;
        void copy_texture_to_buffer(
            const std::shared_ptr<ITexture> &src,
            Offset3d src_offset,
            Extent3d src_extent,
            uint32_t src_mip_level,
            uint32_t src_array_index,
            const std::shared_ptr<IBuffer> &dst,
            uint64_t dst_offset) override;
        void copy_texture_to_texture(
            const std::shared_ptr<ITexture> &src,
            Offset3d src_offset,
            uint32_t src_mip_level,
            uint32_t src_array_index,
            const std::shared_ptr<ITexture> &dst,
            Offset3d dst_offset,
            uint32_t dst_mip_level,
            uint32_t dst_array_index,
            Extent3d extent) override;

        void write_buffer(const std::shared_ptr<IBuffer> &buffer, const void *data, size_t size, uint64_t offset) override;
        void write_texture(
            const std::shared_ptr<ITexture> &texture,
            Offset3d offset,
            Extent3d extent,
            uint32_t mip_level,
            uint32_t array_index,
            const void *data,
            size_t data_size,
            uint64_t data_offset) override;

        std::shared_ptr<IComputePass> begin_compute_pass(const ComputePassDescriptor &descriptor) const override;
        std::shared_ptr<IRenderPass> begin_render_pass(const RenderPassDescriptor &descriptor) const override;

        VkCommandBuffer command_buffer() const;

        static VkPipelineStageFlags2 get_pipeline_stage_flags(BarrierPipelineStage barrier_pipeline_stage);
        static VkAccessFlags2 get_access_flags(BarrierAccess barrier_access);
        static VkImageLayout get_image_layout(BarrierTextureLayout texture_layout);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkCommandBuffer m_command_buffer;
    };
} // namespace hyper_engine