/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/render_pass.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_engine
{
    class VulkanGraphicsDevice;

    class VulkanRenderPass final : public RenderPass
    {
    public:
        VulkanRenderPass(VulkanGraphicsDevice &graphics_device, VkCommandBuffer command_buffer, const RenderPassDescriptor &descriptor);
        ~VulkanRenderPass() override;

        void set_pipeline(const std::shared_ptr<RenderPipeline> &pipeline) override;
        void set_push_constants(const void *data, size_t data_size) const override;

        void set_index_buffer(const std::shared_ptr<Buffer> &buffer) const override;

        void set_scissor(int32_t x, int32_t y, uint32_t width, uint32_t height) const override;
        void set_viewport(float x, float y, float width, float height, float min_depth, float max_depth) const override;

        void draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance) const override;
        void draw_indexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index, int32_t vertex_offset, uint32_t first_instance)
            const override;

        std::string_view label() const override;
        LabelColor label_color() const override;

        const std::vector<ColorAttachment> &color_attachments() const override;
        DepthStencilAttachment depth_stencil_attachment() const override;

        VkCommandBuffer command_buffer() const;

        static VkAttachmentLoadOp get_attachment_load_operation(LoadOperation load_operation);
        static VkAttachmentStoreOp get_attachment_store_operation(StoreOperation store_operation);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        std::string m_label;
        LabelColor m_label_color;

        std::vector<ColorAttachment> m_color_attachments;
        DepthStencilAttachment m_depth_stencil_attachment;

        VkCommandBuffer m_command_buffer;

        std::shared_ptr<RenderPipeline> m_pipeline;
    };
} // namespace hyper_engine