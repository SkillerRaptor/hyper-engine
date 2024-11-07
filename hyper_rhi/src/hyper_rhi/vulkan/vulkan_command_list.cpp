/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_command_list.hpp"

#include "hyper_rhi/vulkan/vulkan_buffer.hpp"
#include "hyper_rhi/vulkan/vulkan_compute_pass.hpp"
#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"
#include "hyper_rhi/vulkan/vulkan_render_pass.hpp"
#include "hyper_rhi/vulkan/vulkan_texture.hpp"

namespace hyper_rhi
{
    VulkanCommandList::VulkanCommandList(VulkanGraphicsDevice &graphics_device)
        : m_graphics_device(graphics_device)
        , m_command_buffer(VK_NULL_HANDLE)
    {
    }

    void VulkanCommandList::begin()
    {
        m_command_buffer = m_graphics_device.current_frame().command_buffer;

        HE_VK_CHECK(vkResetCommandBuffer(m_command_buffer, 0));

        constexpr VkCommandBufferBeginInfo command_buffer_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            .pInheritanceInfo = nullptr,
        };

        HE_VK_CHECK(vkBeginCommandBuffer(m_command_buffer, &command_buffer_begin_info));
    }

    void VulkanCommandList::end()
    {
        HE_VK_CHECK(vkEndCommandBuffer(m_command_buffer));
    }

    void VulkanCommandList::insert_barriers(const Barriers &barriers) const
    {
        std::vector<VkMemoryBarrier2> memory_barriers;
        if (!barriers.memory_barriers.empty())
        {
            for (const MemoryBarrier &memory_barrier : barriers.memory_barriers)
            {
                const VkPipelineStageFlags2 src_stage = VulkanCommandList::get_pipeline_stage_flags(memory_barrier.stage_before);
                const VkAccessFlags2 src_access = VulkanCommandList::get_access_flags(memory_barrier.access_before);
                const VkPipelineStageFlags2 dst_stage = VulkanCommandList::get_pipeline_stage_flags(memory_barrier.stage_after);
                const VkAccessFlags2 dst_access = VulkanCommandList::get_access_flags(memory_barrier.access_after);

                const VkMemoryBarrier2 memory_barrier_descriptor = {
                    .sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2,
                    .pNext = nullptr,
                    .srcStageMask = src_stage,
                    .srcAccessMask = src_access,
                    .dstStageMask = dst_stage,
                    .dstAccessMask = dst_access,
                };

                memory_barriers.push_back(memory_barrier_descriptor);
            }
        }

        std::vector<VkBufferMemoryBarrier2> buffer_memory_barriers;
        if (!barriers.buffer_memory_barriers.empty())
        {
            for (const BufferMemoryBarrier &buffer_memory_barrier : barriers.buffer_memory_barriers)
            {
                const auto buffer = std::dynamic_pointer_cast<VulkanBuffer>(buffer_memory_barrier.buffer);

                const VkPipelineStageFlags2 src_stage = VulkanCommandList::get_pipeline_stage_flags(buffer_memory_barrier.stage_before);
                const VkAccessFlags2 src_access = VulkanCommandList::get_access_flags(buffer_memory_barrier.access_before);
                const VkPipelineStageFlags2 dst_stage = VulkanCommandList::get_pipeline_stage_flags(buffer_memory_barrier.stage_after);
                const VkAccessFlags2 dst_access = VulkanCommandList::get_access_flags(buffer_memory_barrier.access_after);

                const VkBufferMemoryBarrier2 buffer_memory_barrier_descriptor = {
                    .sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2,
                    .pNext = nullptr,
                    .srcStageMask = src_stage,
                    .srcAccessMask = src_access,
                    .dstStageMask = dst_stage,
                    .dstAccessMask = dst_access,
                    .srcQueueFamilyIndex = 0,
                    .dstQueueFamilyIndex = 0,
                    .buffer = buffer->buffer(),
                    .offset = 0,
                    .size = VK_WHOLE_SIZE,
                };

                buffer_memory_barriers.push_back(buffer_memory_barrier_descriptor);
            }
        }

        std::vector<VkImageMemoryBarrier2> image_memory_barriers;
        if (!barriers.texture_memory_barriers.empty())
        {
            for (const TextureMemoryBarrier &texture_memory_barrier : barriers.texture_memory_barriers)
            {
                const auto texture = std::dynamic_pointer_cast<VulkanTexture>(texture_memory_barrier.texture);

                const VkPipelineStageFlags2 src_stage = VulkanCommandList::get_pipeline_stage_flags(texture_memory_barrier.stage_before);
                const VkAccessFlags2 src_access = VulkanCommandList::get_access_flags(texture_memory_barrier.access_before);
                const VkPipelineStageFlags2 dst_stage = VulkanCommandList::get_pipeline_stage_flags(texture_memory_barrier.stage_after);
                const VkAccessFlags2 dst_access = VulkanCommandList::get_access_flags(texture_memory_barrier.access_after);

                const VkImageLayout old_layout = VulkanCommandList::get_image_layout(texture_memory_barrier.layout_before);
                const VkImageLayout new_layout = VulkanCommandList::get_image_layout(texture_memory_barrier.layout_after);

                const VkImageSubresourceRange subresource_range = {
                    .aspectMask = VulkanTextureView::get_image_aspect_flags(texture->format()),
                    .baseMipLevel = texture_memory_barrier.subresource_range.base_mip_level,
                    .levelCount = texture_memory_barrier.subresource_range.mip_level_count,
                    .baseArrayLayer = texture_memory_barrier.subresource_range.base_array_level,
                    .layerCount = texture_memory_barrier.subresource_range.array_layer_count,
                };

                const VkImageMemoryBarrier2 texture_memory_barrier_descriptor = {
                    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
                    .pNext = nullptr,
                    .srcStageMask = src_stage,
                    .srcAccessMask = src_access,
                    .dstStageMask = dst_stage,
                    .dstAccessMask = dst_access,
                    .oldLayout = old_layout,
                    .newLayout = new_layout,
                    .srcQueueFamilyIndex = 0,
                    .dstQueueFamilyIndex = 0,
                    .image = texture->image(),
                    .subresourceRange = subresource_range,
                };

                image_memory_barriers.push_back(texture_memory_barrier_descriptor);
            }
        }

        const VkDependencyInfo dependency_info = {
            .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
            .pNext = nullptr,
            .dependencyFlags = 0,
            .memoryBarrierCount = static_cast<uint32_t>(memory_barriers.size()),
            .pMemoryBarriers = memory_barriers.data(),
            .bufferMemoryBarrierCount = static_cast<uint32_t>(buffer_memory_barriers.size()),
            .pBufferMemoryBarriers = buffer_memory_barriers.data(),
            .imageMemoryBarrierCount = static_cast<uint32_t>(image_memory_barriers.size()),
            .pImageMemoryBarriers = image_memory_barriers.data(),
        };

        vkCmdPipelineBarrier2(m_command_buffer, &dependency_info);
    }

    void VulkanCommandList::clear_buffer(const std::shared_ptr<Buffer> &buffer, const size_t size, const uint64_t offset)
    {
        HE_UNUSED(buffer);
        HE_UNUSED(size);
        HE_UNUSED(offset);

        HE_PANIC();
    }

    void VulkanCommandList::clear_texture(const std::shared_ptr<Texture> &texture, const TextureSubresourceRange subresource_range)
    {
        HE_UNUSED(texture);
        HE_UNUSED(subresource_range);

        HE_PANIC();
    }

    void VulkanCommandList::copy_buffer_to_buffer(
        const std::shared_ptr<Buffer> &src,
        const uint64_t src_offset,
        const std::shared_ptr<Buffer> &dst,
        const uint64_t dst_offset,
        const size_t size)
    {
        HE_UNUSED(src);
        HE_UNUSED(src_offset);
        HE_UNUSED(dst);
        HE_UNUSED(dst_offset);
        HE_UNUSED(size);

        HE_PANIC();
    }

    void VulkanCommandList::copy_buffer_to_texture(
        const std::shared_ptr<Buffer> &src,
        const uint64_t src_offset,
        const std::shared_ptr<Texture> &dst,
        const Offset3d dst_offset,
        const Extent3d dst_extent,
        const uint32_t dst_mip_level,
        const uint32_t dst_array_index)
    {
        HE_UNUSED(src);
        HE_UNUSED(src_offset);
        HE_UNUSED(dst);
        HE_UNUSED(dst_offset);
        HE_UNUSED(dst_extent);
        HE_UNUSED(dst_mip_level);
        HE_UNUSED(dst_array_index);

        HE_PANIC();
    }

    void VulkanCommandList::copy_texture_to_buffer(
        const std::shared_ptr<Texture> &src,
        const Offset3d src_offset,
        const Extent3d src_extent,
        const uint32_t src_mip_level,
        const uint32_t src_array_index,
        const std::shared_ptr<Buffer> &dst,
        const uint64_t dst_offset)
    {
        HE_UNUSED(src);
        HE_UNUSED(src_offset);
        HE_UNUSED(src_extent);
        HE_UNUSED(src_mip_level);
        HE_UNUSED(src_array_index);
        HE_UNUSED(dst);
        HE_UNUSED(dst_offset);

        HE_PANIC();
    }

    void VulkanCommandList::copy_texture_to_texture(
        const std::shared_ptr<Texture> &src,
        const Offset3d src_offset,
        const uint32_t src_mip_level,
        const uint32_t src_array_index,
        const std::shared_ptr<Texture> &dst,
        const Offset3d dst_offset,
        const uint32_t dst_mip_level,
        const uint32_t dst_array_index,
        const Extent3d extent)
    {
        const auto vulkan_src = std::dynamic_pointer_cast<VulkanTexture>(src);
        const auto vulkan_dst = std::dynamic_pointer_cast<VulkanTexture>(dst);
        const VkImageCopy2 image_copy_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_COPY_2,
            .pNext = nullptr,
            .srcSubresource =
                VkImageSubresourceLayers{
                    .aspectMask = VulkanTextureView::get_image_aspect_flags(vulkan_src->format()),
                    .mipLevel = src_mip_level,
                    .baseArrayLayer = src_array_index,
                    .layerCount = 1,
                },
            .srcOffset =
                VkOffset3D{
                    .x = src_offset.x,
                    .y = src_offset.y,
                    .z = src_offset.z,
                },
            .dstSubresource =
                VkImageSubresourceLayers{
                    .aspectMask = VulkanTextureView::get_image_aspect_flags(vulkan_dst->format()),
                    .mipLevel = dst_mip_level,
                    .baseArrayLayer = dst_array_index,
                    .layerCount = 1,
                },
            .dstOffset =
                VkOffset3D{
                    .x = dst_offset.x,
                    .y = dst_offset.y,
                    .z = dst_offset.z,
                },
            .extent =
                VkExtent3D{
                    .width = extent.width,
                    .height = extent.height,
                    .depth = extent.depth,
                },
        };

        const VkCopyImageInfo2 copy_image_info = {
            .sType = VK_STRUCTURE_TYPE_COPY_IMAGE_INFO_2,
            .pNext = nullptr,
            .srcImage = vulkan_src->image(),
            .srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            .dstImage = vulkan_dst->image(),
            .dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            .regionCount = 1,
            .pRegions = &image_copy_info,
        };

        vkCmdCopyImage2(m_command_buffer, &copy_image_info);
    }

    void VulkanCommandList::write_buffer(const std::shared_ptr<Buffer> &buffer, const void *data, const size_t size, const uint64_t offset)
    {
        const auto vulkan_buffer = std::dynamic_pointer_cast<VulkanBuffer>(buffer);

        if (buffer->byte_size() <= 65535)
        {
            vkCmdUpdateBuffer(m_command_buffer, vulkan_buffer->buffer(), offset, size, data);
            HE_TRACE("Uploading {} bytes to {} buffer", size, buffer->label().empty() ? "a" : fmt::format("the '{}'", buffer->label()));
        }
        else
        {
            const std::shared_ptr<Buffer> staging_buffer = m_graphics_device.create_staging_buffer({
                .label = fmt::format("{} Staging", buffer->label()),
                .byte_size = static_cast<uint64_t>(size),
                .usage = BufferUsage::Storage,
            });
            const std::shared_ptr<VulkanBuffer> vulkan_staging_buffer = std::dynamic_pointer_cast<VulkanBuffer>(staging_buffer);

            void *mapped_ptr = nullptr;
            vmaMapMemory(m_graphics_device.allocator(), vulkan_staging_buffer->allocation(), &mapped_ptr);
            memcpy(mapped_ptr, data, size);
            vmaUnmapMemory(m_graphics_device.allocator(), vulkan_staging_buffer->allocation());

            const VkBufferCopy2 buffer_copy_info = {
                .sType = VK_STRUCTURE_TYPE_BUFFER_COPY_2,
                .pNext = nullptr,
                .srcOffset = 0,
                .dstOffset = offset,
                .size = static_cast<uint64_t>(size),
            };

            const VkCopyBufferInfo2 copy_buffer_info = {
                .sType = VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2,
                .pNext = nullptr,
                .srcBuffer = vulkan_staging_buffer->buffer(),
                .dstBuffer = vulkan_buffer->buffer(),
                .regionCount = 1,
                .pRegions = &buffer_copy_info,
            };

            vkCmdCopyBuffer2(m_command_buffer, &copy_buffer_info);

            HE_TRACE(
                "Uploading via staging buffer {} bytes to {} buffer",
                size,
                buffer->label().empty() ? "a" : fmt::format("the '{}'", buffer->label()));
        }
    }

    void VulkanCommandList::write_texture(
        const std::shared_ptr<Texture> &texture,
        const Offset3d offset,
        const Extent3d extent,
        const uint32_t mip_level,
        const uint32_t array_index,
        const void *data,
        const size_t data_size,
        const uint64_t data_offset)
    {
        HE_UNUSED(texture);
        HE_UNUSED(offset);
        HE_UNUSED(extent);
        HE_UNUSED(mip_level);
        HE_UNUSED(array_index);
        HE_UNUSED(data);
        HE_UNUSED(data_size);
        HE_UNUSED(data_offset);

        HE_PANIC();
    }

    std::shared_ptr<ComputePass> VulkanCommandList::begin_compute_pass(const ComputePassDescriptor &descriptor) const
    {
        return std::make_shared<VulkanComputePass>(m_graphics_device, m_command_buffer, descriptor);
    }

    std::shared_ptr<RenderPass> VulkanCommandList::begin_render_pass(const RenderPassDescriptor &descriptor) const
    {
        return std::make_shared<VulkanRenderPass>(m_graphics_device, m_command_buffer, descriptor);
    }

    VkPipelineStageFlags2 VulkanCommandList::get_pipeline_stage_flags(const BarrierPipelineStage barrier_pipeline_stage)
    {
        VkPipelineStageFlags2 pipeline_stage = VK_PIPELINE_STAGE_2_NONE;

        if ((barrier_pipeline_stage & BarrierPipelineStage::ComputeShader) == BarrierPipelineStage::ComputeShader)
        {
            pipeline_stage |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
        }

        if ((barrier_pipeline_stage & BarrierPipelineStage::VertexShader) == BarrierPipelineStage::VertexShader)
        {
            pipeline_stage |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT;
        }

        if ((barrier_pipeline_stage & BarrierPipelineStage::FragmentShader) == BarrierPipelineStage::FragmentShader)
        {
            pipeline_stage |= VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
        }

        if ((barrier_pipeline_stage & BarrierPipelineStage::EarlyFragmentTests) == BarrierPipelineStage::EarlyFragmentTests)
        {
            pipeline_stage |= VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT;
        }

        if ((barrier_pipeline_stage & BarrierPipelineStage::LateFragmentTests) == BarrierPipelineStage::LateFragmentTests)
        {
            pipeline_stage |= VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
        }

        if ((barrier_pipeline_stage & BarrierPipelineStage::ColorAttachmentOutput) == BarrierPipelineStage::ColorAttachmentOutput)
        {
            pipeline_stage |= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
        }

        if ((barrier_pipeline_stage & BarrierPipelineStage::Copy) == BarrierPipelineStage::Copy)
        {
            pipeline_stage |= VK_PIPELINE_STAGE_2_COPY_BIT;
        }

        if ((barrier_pipeline_stage & BarrierPipelineStage::Clear) == BarrierPipelineStage::Clear)
        {
            pipeline_stage |= VK_PIPELINE_STAGE_2_CLEAR_BIT;
        }

        if ((barrier_pipeline_stage & BarrierPipelineStage::AllGraphics) == BarrierPipelineStage::AllGraphics)
        {
            pipeline_stage |= VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT;
        }

        if ((barrier_pipeline_stage & BarrierPipelineStage::AllTransfer) == BarrierPipelineStage::AllTransfer)
        {
            pipeline_stage |= VK_PIPELINE_STAGE_2_ALL_TRANSFER_BIT;
        }

        if ((barrier_pipeline_stage & BarrierPipelineStage::AllCommands) == BarrierPipelineStage::AllCommands)
        {
            pipeline_stage |= VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
        }

        return pipeline_stage;
    }

    VkAccessFlags2 VulkanCommandList::get_access_flags(BarrierAccess barrier_access)
    {
        VkAccessFlags2 access = VK_ACCESS_2_NONE;

        if ((barrier_access & BarrierAccess::ShaderRead) == BarrierAccess::ShaderRead)
        {
            access |= VK_ACCESS_2_SHADER_READ_BIT;
        }

        if ((barrier_access & BarrierAccess::ShaderWrite) == BarrierAccess::ShaderWrite)
        {
            access |= VK_ACCESS_2_SHADER_WRITE_BIT;
        }

        if ((barrier_access & BarrierAccess::ColorAttachmentRead) == BarrierAccess::ColorAttachmentRead)
        {
            access |= VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT;
        }

        if ((barrier_access & BarrierAccess::ColorAttachmentWrite) == BarrierAccess::ColorAttachmentWrite)
        {
            access |= VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
        }

        if ((barrier_access & BarrierAccess::DepthStencilAttachmentRead) == BarrierAccess::DepthStencilAttachmentRead)
        {
            access |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        }

        if ((barrier_access & BarrierAccess::DepthStencilAttachmentWrite) == BarrierAccess::DepthStencilAttachmentWrite)
        {
            access |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        }

        if ((barrier_access & BarrierAccess::TransferRead) == BarrierAccess::TransferRead)
        {
            access |= VK_ACCESS_2_TRANSFER_READ_BIT;
        }

        if ((barrier_access & BarrierAccess::TransferWrite) == BarrierAccess::TransferWrite)
        {
            access |= VK_ACCESS_2_TRANSFER_WRITE_BIT;
        }

        return access;
    }

    VkImageLayout VulkanCommandList::get_image_layout(const BarrierTextureLayout texture_layout)
    {
        switch (texture_layout)
        {
        case BarrierTextureLayout::Undefined:
            return VK_IMAGE_LAYOUT_UNDEFINED;
        case BarrierTextureLayout::General:
            return VK_IMAGE_LAYOUT_GENERAL;
        case BarrierTextureLayout::Present:
            return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        case BarrierTextureLayout::ColorAttachment:
            return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        case BarrierTextureLayout::DepthStencilAttachment:
            return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
        case BarrierTextureLayout::ShaderReadOnly:
            return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        case BarrierTextureLayout::TransferSrc:
            return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        case BarrierTextureLayout::TransferDst:
            return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        default:
            HE_UNREACHABLE();
        }
    }
} // namespace hyper_rhi