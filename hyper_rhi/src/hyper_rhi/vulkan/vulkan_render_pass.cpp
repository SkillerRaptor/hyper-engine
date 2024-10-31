/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_render_pass.hpp"

#include "hyper_rhi/vulkan/vulkan_buffer.hpp"
#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"
#include "hyper_rhi/vulkan/vulkan_graphics_pipeline.hpp"
#include "hyper_rhi/vulkan/vulkan_pipeline_layout.hpp"
#include "hyper_rhi/vulkan/vulkan_texture.hpp"

namespace hyper_rhi
{
    VulkanRenderPass::VulkanRenderPass(
        VulkanGraphicsDevice &graphics_device,
        const VkCommandBuffer command_buffer,
        const RenderPassDescriptor &descriptor)
        : RenderPass(descriptor)
        , m_graphics_device(graphics_device)
        , m_command_buffer(command_buffer)
        , m_graphics_pipeline(nullptr)
    {
        m_graphics_device.begin_marker(m_command_buffer, MarkerType::RenderPass, m_label, m_label_color);

        const auto color_attachment = std::dynamic_pointer_cast<VulkanTexture>(m_color_attachment.attachment);
        const auto depth_attachment = std::dynamic_pointer_cast<VulkanTexture>(m_depth_attachment.attachment);

        const VkExtent2D render_area_extent = {
            .width = color_attachment->width(),
            .height = color_attachment->height(),
        };

        constexpr VkOffset2D render_area_offset = {
            .x = 0,
            .y = 0,
        };

        const VkRect2D render_area = {
            .offset = render_area_offset,
            .extent = render_area_extent,
        };

        constexpr VkClearValue clear_value = {
            .color = {
                .float32 = {
                    0.0f,
                    0.0f,
                    0.0f,
                    1.0f,
                },
            },
        };

        const auto &color_attachment_view = dynamic_cast<VulkanTextureView &>(color_attachment->view());
        const VkRenderingAttachmentInfo color_attachment_info = {
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .pNext = nullptr,
            .imageView = color_attachment_view.image_view(),
            .imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
            .resolveMode = VK_RESOLVE_MODE_NONE,
            .resolveImageView = VK_NULL_HANDLE,
            .resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .loadOp = VulkanRenderPass::get_load_operation(descriptor.color_attachment.operation.load_operation),
            .storeOp = VulkanRenderPass::get_store_operation(descriptor.color_attachment.operation.store_operation),
            .clearValue = clear_value,
        };

        constexpr VkClearValue depth_clear_value = {
            .depthStencil = {
                .depth = 1.0,
                .stencil = 0,
            },
        };

        const auto &depth_attachment_view = dynamic_cast<VulkanTextureView &>(depth_attachment->view());
        const VkRenderingAttachmentInfo depth_attachment_info = {
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .pNext = nullptr,
            .imageView = depth_attachment_view.image_view(),
            .imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
            .resolveMode = VK_RESOLVE_MODE_NONE,
            .resolveImageView = VK_NULL_HANDLE,
            .resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .loadOp = VulkanRenderPass::get_load_operation(descriptor.depth_attachment.operation.load_operation),
            .storeOp = VulkanRenderPass::get_store_operation(descriptor.depth_attachment.operation.store_operation),
            .clearValue = depth_clear_value,
        };

        const VkRenderingInfo rendering_info = {
            .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
            .pNext = nullptr,
            .flags = 0,
            .renderArea = render_area,
            .layerCount = 1,
            .viewMask = 0,
            .colorAttachmentCount = 1,
            .pColorAttachments = &color_attachment_info,
            .pDepthAttachment = &depth_attachment_info,
            .pStencilAttachment = nullptr,
        };

        const VkViewport viewport = {
            .x = 0.0,
            .y = 0,
            .width = static_cast<float>(render_area_extent.width),
            .height = static_cast<float>(render_area_extent.height),
            .minDepth = 0.0,
            .maxDepth = 1.0,
        };

        constexpr VkOffset2D offset = {
            .x = 0,
            .y = 0,
        };

        const VkRect2D scissor = {
            .offset = offset,
            .extent = render_area_extent,
        };

        vkCmdBeginRendering(m_command_buffer, &rendering_info);
        vkCmdSetViewport(m_command_buffer, 0, 1, &viewport);
        vkCmdSetScissor(m_command_buffer, 0, 1, &scissor);
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        vkCmdEndRendering(m_command_buffer);

        m_graphics_device.end_marker(m_command_buffer);
    }

    void VulkanRenderPass::set_pipeline(const std::shared_ptr<GraphicsPipeline> &pipeline)
    {
        m_graphics_pipeline = pipeline;

        const std::shared_ptr<VulkanGraphicsPipeline> vulkan_pipeline = std::dynamic_pointer_cast<VulkanGraphicsPipeline>(m_graphics_pipeline);
        const auto &layout = dynamic_cast<VulkanPipelineLayout &>(m_graphics_pipeline->layout());

        vkCmdBindDescriptorSets(
            m_command_buffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            layout.pipeline_layout(),
            0,
            static_cast<uint32_t>(m_graphics_device.descriptor_manager().descriptor_sets().size()),
            m_graphics_device.descriptor_manager().descriptor_sets().data(),
            0,
            nullptr);

        vkCmdBindPipeline(m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan_pipeline->pipeline());
    }

    void VulkanRenderPass::set_index_buffer(const std::shared_ptr<Buffer> &buffer) const
    {
        const auto vulkan_buffer = std::dynamic_pointer_cast<VulkanBuffer>(buffer);

        vkCmdBindIndexBuffer(m_command_buffer, vulkan_buffer->buffer(), 0, VK_INDEX_TYPE_UINT32);
    }

    void VulkanRenderPass::set_push_constants(const void *data, const size_t data_size) const
    {
        const auto &layout = dynamic_cast<VulkanPipelineLayout &>(m_graphics_pipeline->layout());

        vkCmdPushConstants(m_command_buffer, layout.pipeline_layout(), VK_SHADER_STAGE_ALL, 0, static_cast<uint32_t>(data_size), data);
    }

    void VulkanRenderPass::draw(const DrawArguments &arguments) const
    {
        vkCmdDraw(m_command_buffer, arguments.vertex_count, arguments.instance_count, arguments.first_vertex, arguments.first_instance);
    }

    void VulkanRenderPass::draw_indexed(const DrawIndexedArguments &arguments) const
    {
        vkCmdDrawIndexed(
            m_command_buffer,
            arguments.index_count,
            arguments.instance_count,
            arguments.first_index,
            arguments.vertex_offset,
            arguments.first_instance);
    }

    VkAttachmentLoadOp VulkanRenderPass::get_load_operation(const LoadOperation load_operation)
    {
        switch (load_operation)
        {
        case Clear:
            return VK_ATTACHMENT_LOAD_OP_CLEAR;
        case Load:
            return VK_ATTACHMENT_LOAD_OP_LOAD;
        default:
            HE_UNREACHABLE();
        }
    }

    VkAttachmentStoreOp VulkanRenderPass::get_store_operation(const StoreOperation store_operation)
    {
        switch (store_operation)
        {
        case Store:
            return VK_ATTACHMENT_STORE_OP_STORE;
        case Discard:
            return VK_ATTACHMENT_STORE_OP_DONT_CARE;
        default:
            HE_UNREACHABLE();
        }
    }
} // namespace hyper_rhi