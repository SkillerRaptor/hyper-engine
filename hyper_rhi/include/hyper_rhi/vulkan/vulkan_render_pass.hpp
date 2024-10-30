/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/render_pass.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanRenderPass final : public RenderPass
    {
    public:
        VulkanRenderPass(VulkanGraphicsDevice &graphics_device, VkCommandBuffer command_buffer, const RenderPassDescriptor &descriptor);
        ~VulkanRenderPass() override;

        void set_pipeline(const GraphicsPipelineHandle &pipeline_handle) override;
        void set_index_buffer(BufferHandle buffer_handle) const override;
        void set_push_constants(const void *data, size_t data_size) const override;

        void draw(const DrawArguments &arguments) const override;
        void draw_indexed(const DrawIndexedArguments &arguments) const override;

    private:
        [[nodiscard]] static VkAttachmentLoadOp get_load_operation(LoadOperation load_operation);
        [[nodiscard]] static VkAttachmentStoreOp get_store_operation(StoreOperation store_operation);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkCommandBuffer m_command_buffer;

        TextureHandle m_color_attachment;
        TextureHandle m_depth_attachment;
        GraphicsPipelineHandle m_graphics_pipeline;
    };
} // namespace hyper_rhi