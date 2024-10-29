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
        : RenderPass(descriptor.label)
        , m_graphics_device(graphics_device)
        , m_command_buffer(command_buffer)
        , m_color_attachment(descriptor.color_attachment)
        , m_graphics_pipeline(nullptr)
    {
        const std::shared_ptr<VulkanTexture> texture = std::dynamic_pointer_cast<VulkanTexture>(m_color_attachment);

        constexpr VkImageSubresourceRange subresource_range = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        };

        const VkImageMemoryBarrier2 image_memory_barrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
            .pNext = nullptr,
            .srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
            .srcAccessMask = VK_ACCESS_2_NONE,
            .dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
            .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .srcQueueFamilyIndex = 0,
            .dstQueueFamilyIndex = 0,
            .image = texture->image(),
            .subresourceRange = subresource_range,
        };

        const VkDependencyInfo dependency_info = {
            .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
            .pNext = nullptr,
            .dependencyFlags = 0,
            .memoryBarrierCount = 0,
            .pMemoryBarriers = nullptr,
            .bufferMemoryBarrierCount = 0,
            .pBufferMemoryBarriers = nullptr,
            .imageMemoryBarrierCount = 1,
            .pImageMemoryBarriers = &image_memory_barrier,
        };

        vkCmdPipelineBarrier2(m_command_buffer, &dependency_info);

        const VkExtent2D render_area_extent = {
            .width = texture->width(),
            .height = texture->height(),
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
                    static_cast<float>(0.0),
                    static_cast<float>(0.2),
                    static_cast<float>(0.4),
                    static_cast<float>(1.0),
                },
            },
        };

        const VkRenderingAttachmentInfo color_attachment_info = {
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .pNext = nullptr,
            .imageView = texture->view(),
            .imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
            .resolveMode = VK_RESOLVE_MODE_NONE,
            .resolveImageView = VK_NULL_HANDLE,
            .resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .clearValue = clear_value,
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
            .pDepthAttachment = nullptr,
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
        const std::shared_ptr<VulkanTexture> texture = std::dynamic_pointer_cast<VulkanTexture>(m_color_attachment);

        vkCmdEndRendering(m_command_buffer);

        constexpr VkImageSubresourceRange subresource_range = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        };

        const VkImageMemoryBarrier2 image_memory_barrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
            .pNext = nullptr,
            .srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
            .dstAccessMask = VK_ACCESS_2_NONE,
            .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            .srcQueueFamilyIndex = 0,
            .dstQueueFamilyIndex = 0,
            .image = texture->image(),
            .subresourceRange = subresource_range,
        };

        const VkDependencyInfo dependency_info = {
            .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
            .pNext = nullptr,
            .dependencyFlags = 0,
            .memoryBarrierCount = 0,
            .pMemoryBarriers = nullptr,
            .bufferMemoryBarrierCount = 0,
            .pBufferMemoryBarriers = nullptr,
            .imageMemoryBarrierCount = 1,
            .pImageMemoryBarriers = &image_memory_barrier,
        };

        vkCmdPipelineBarrier2(m_command_buffer, &dependency_info);
    }

    void VulkanRenderPass::set_pipeline(const GraphicsPipelineHandle &pipeline_handle)
    {
        m_graphics_pipeline = pipeline_handle;

        const std::shared_ptr<VulkanGraphicsPipeline> pipeline = std::dynamic_pointer_cast<VulkanGraphicsPipeline>(m_graphics_pipeline);
        const std::shared_ptr<VulkanPipelineLayout> layout = std::dynamic_pointer_cast<VulkanPipelineLayout>(m_graphics_pipeline->layout());

        vkCmdBindDescriptorSets(
            m_command_buffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            layout->pipeline_layout(),
            0,
            static_cast<uint32_t>(m_graphics_device.descriptor_manager().descriptor_sets().size()),
            m_graphics_device.descriptor_manager().descriptor_sets().data(),
            0,
            nullptr);

        vkCmdBindPipeline(m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline());
    }

    void VulkanRenderPass::set_index_buffer(const BufferHandle buffer_handle) const
    {
        const std::shared_ptr<VulkanBuffer> buffer = std::dynamic_pointer_cast<VulkanBuffer>(buffer_handle);

        vkCmdBindIndexBuffer(m_command_buffer, buffer->buffer(), 0, VK_INDEX_TYPE_UINT32);
    }

    void VulkanRenderPass::set_push_constants(const void *data, const size_t data_size) const
    {
        const std::shared_ptr<VulkanPipelineLayout> layout = std::dynamic_pointer_cast<VulkanPipelineLayout>(m_graphics_pipeline->layout());

        vkCmdPushConstants(m_command_buffer, layout->pipeline_layout(), VK_SHADER_STAGE_ALL, 0, static_cast<uint32_t>(data_size), data);
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
} // namespace hyper_rhi