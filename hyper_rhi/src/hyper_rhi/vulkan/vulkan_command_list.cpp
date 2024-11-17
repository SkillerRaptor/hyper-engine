/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_command_list.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_core/logger.hpp>

#include "hyper_rhi/vulkan/vulkan_buffer.hpp"
#include "hyper_rhi/vulkan/vulkan_compute_pass.hpp"
#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"
#include "hyper_rhi/vulkan/vulkan_render_pass.hpp"
#include "hyper_rhi/vulkan/vulkan_texture.hpp"
#include "hyper_rhi/vulkan/vulkan_texture_view.hpp"

namespace he::rhi
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
        const auto vulkan_buffer = std::dynamic_pointer_cast<VulkanBuffer>(buffer);

        vkCmdFillBuffer(m_command_buffer, vulkan_buffer->buffer(), offset, size, 0);

        HE_TRACE("Clearing {} buffer", buffer->label().empty() ? "a" : fmt::format("the '{}'", buffer->label()));
    }

    void VulkanCommandList::clear_texture(const std::shared_ptr<Texture> &texture, const SubresourceRange subresource_range)
    {
        const auto vulkan_texture = std::dynamic_pointer_cast<VulkanTexture>(texture);

        const VkImageSubresourceRange vulkan_subresource_range = {
            .aspectMask = VulkanTextureView::get_image_aspect_flags(texture->format()),
            .baseMipLevel = subresource_range.base_mip_level,
            .levelCount = subresource_range.mip_level_count,
            .baseArrayLayer = subresource_range.base_array_level,
            .layerCount = subresource_range.array_layer_count,
        };

        switch (texture->format())
        {
        case Format::R8Unorm:
        case Format::R8Snorm:
        case Format::R8Uint:
        case Format::R8Sint:
        case Format::R8Srgb:
        case Format::Rg8Unorm:
        case Format::Rg8Snorm:
        case Format::Rg8Uint:
        case Format::Rg8Sint:
        case Format::Rg8Srgb:
        case Format::Rgb8Unorm:
        case Format::Rgb8Snorm:
        case Format::Rgb8Uint:
        case Format::Rgb8Sint:
        case Format::Rgb8Srgb:
        case Format::Bgr8Unorm:
        case Format::Bgr8Snorm:
        case Format::Bgr8Uint:
        case Format::Bgr8Sint:
        case Format::Bgr8Srgb:
        case Format::Rgba8Unorm:
        case Format::Rgba8Snorm:
        case Format::Rgba8Uint:
        case Format::Rgba8Sint:
        case Format::Rgba8Srgb:
        case Format::Bgra8Unorm:
        case Format::Bgra8Snorm:
        case Format::Bgra8Uint:
        case Format::Bgra8Sint:
        case Format::Bgra8Srgb:
        case Format::R16Unorm:
        case Format::R16Snorm:
        case Format::R16Uint:
        case Format::R16Sint:
        case Format::R16Sfloat:
        case Format::Rg16Unorm:
        case Format::Rg16Snorm:
        case Format::Rg16Uint:
        case Format::Rg16Sint:
        case Format::Rg16Sfloat:
        case Format::Rgb16Unorm:
        case Format::Rgb16Snorm:
        case Format::Rgb16Uint:
        case Format::Rgb16Sint:
        case Format::Rgb16Sfloat:
        case Format::Rgba16Unorm:
        case Format::Rgba16Snorm:
        case Format::Rgba16Uint:
        case Format::Rgba16Sint:
        case Format::Rgba16Sfloat:
        case Format::R32Uint:
        case Format::R32Sint:
        case Format::R32Sfloat:
        case Format::Rg32Uint:
        case Format::Rg32Sint:
        case Format::Rg32Sfloat:
        case Format::Rgb32Uint:
        case Format::Rgb32Sint:
        case Format::Rgb32Sfloat:
        case Format::Rgba32Uint:
        case Format::Rgba32Sint:
        case Format::Rgba32Sfloat:
        case Format::R64Uint:
        case Format::R64Sint:
        case Format::R64Sfloat:
        case Format::Rg64Uint:
        case Format::Rg64Sint:
        case Format::Rg64Sfloat:
        case Format::Rgb64Uint:
        case Format::Rgb64Sint:
        case Format::Rgb64Sfloat:
        case Format::Rgba64Uint:
        case Format::Rgba64Sint:
        case Format::Rgba64Sfloat:
        {
            constexpr VkClearColorValue clear_color_value = {
                .float32 = {
                    0.0,
                    0.0,
                    0.0,
                    1.0,
                },
            };

            vkCmdClearColorImage(
                m_command_buffer,
                vulkan_texture->image(),
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                &clear_color_value,
                1,
                &vulkan_subresource_range);
            break;
        }
        case Format::D16Unorm:
        case Format::D32Sfloat:
        case Format::S8Uint:
        case Format::D16UnormS8Uint:
        case Format::D24UnormS8Uint:
        case Format::D32SfloatS8Uint:
        {
            constexpr VkClearDepthStencilValue clear_depth_stencil_value = {
                .depth = 1.0,
                .stencil = 0,
            };

            vkCmdClearDepthStencilImage(
                m_command_buffer,
                vulkan_texture->image(),
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                &clear_depth_stencil_value,
                1,
                &vulkan_subresource_range);
            break;
        }
        case Format::Unknown:
        default:
            HE_UNREACHABLE();
        }

        HE_TRACE("Clearing {} texture", texture->label().empty() ? "a" : fmt::format("the '{}'", texture->label()));
    }

    void VulkanCommandList::copy_buffer_to_buffer(
        const std::shared_ptr<Buffer> &src,
        const uint64_t src_offset,
        const std::shared_ptr<Buffer> &dst,
        const uint64_t dst_offset,
        const size_t size)
    {
        const auto vulkan_src = std::dynamic_pointer_cast<VulkanBuffer>(src);
        const auto vulkan_dst = std::dynamic_pointer_cast<VulkanBuffer>(dst);

        const VkBufferCopy2 region = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_COPY_2,
            .pNext = nullptr,
            .srcOffset = src_offset,
            .dstOffset = dst_offset,
            .size = size,
        };

        const VkCopyBufferInfo2 copy_buffer_info = {
            .sType = VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2,
            .pNext = nullptr,
            .srcBuffer = vulkan_src->buffer(),
            .dstBuffer = vulkan_dst->buffer(),
            .regionCount = 1,
            .pRegions = &region,
        };

        vkCmdCopyBuffer2(m_command_buffer, &copy_buffer_info);

        HE_TRACE(
            "Copying {} buffer to {} buffer",
            src->label().empty() ? "a" : fmt::format("the '{}'", src->label()),
            dst->label().empty() ? "a" : fmt::format("the '{}'", dst->label()));
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
        const auto vulkan_src = std::dynamic_pointer_cast<VulkanBuffer>(src);
        const auto vulkan_dst = std::dynamic_pointer_cast<VulkanTexture>(dst);

        const VkImageSubresourceLayers subresource_layers = {
            .aspectMask = VulkanTextureView::get_image_aspect_flags(dst->format()),
            .mipLevel = dst_mip_level,
            .baseArrayLayer = dst_array_index,
            .layerCount = 1,
        };

        const VkBufferImageCopy2 region = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2,
            .pNext = nullptr,
            .bufferOffset = src_offset,
            .bufferRowLength = 0,
            .bufferImageHeight = 0,
            .imageSubresource = subresource_layers,
            .imageOffset =
                VkOffset3D{
                    .x = dst_offset.x,
                    .y = dst_offset.y,
                    .z = dst_offset.z,
                },
            .imageExtent =
                VkExtent3D{
                    .width = dst_extent.width,
                    .height = dst_extent.height,
                    .depth = dst_extent.depth,
                },
        };

        const VkCopyBufferToImageInfo2 copy_buffer_to_image_info = {
            .sType = VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2,
            .pNext = nullptr,
            .srcBuffer = vulkan_src->buffer(),
            .dstImage = vulkan_dst->image(),
            .dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            .regionCount = 1,
            .pRegions = &region,
        };

        vkCmdCopyBufferToImage2(m_command_buffer, &copy_buffer_to_image_info);

        HE_TRACE(
            "Copying {} buffer to {} texture",
            src->label().empty() ? "a" : fmt::format("the '{}'", src->label()),
            dst->label().empty() ? "a" : fmt::format("the '{}'", dst->label()));
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
        const auto vulkan_src = std::dynamic_pointer_cast<VulkanTexture>(src);
        const auto vulkan_dst = std::dynamic_pointer_cast<VulkanBuffer>(dst);

        const VkImageSubresourceLayers subresource_layers = {
            .aspectMask = VulkanTextureView::get_image_aspect_flags(src->format()),
            .mipLevel = src_mip_level,
            .baseArrayLayer = src_array_index,
            .layerCount = 1,
        };

        const VkBufferImageCopy2 region = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2,
            .pNext = nullptr,
            .bufferOffset = dst_offset,
            .bufferRowLength = 0,
            .bufferImageHeight = 0,
            .imageSubresource = subresource_layers,
            .imageOffset =
                VkOffset3D{
                    .x = src_offset.x,
                    .y = src_offset.y,
                    .z = src_offset.z,
                },
            .imageExtent =
                VkExtent3D{
                    .width = src_extent.width,
                    .height = src_extent.height,
                    .depth = src_extent.depth,
                },
        };

        const VkCopyImageToBufferInfo2 copy_image_to_buffer_info = {
            .sType = VK_STRUCTURE_TYPE_COPY_IMAGE_TO_BUFFER_INFO_2,
            .pNext = nullptr,
            .srcImage = vulkan_src->image(),
            .srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            .dstBuffer = vulkan_dst->buffer(),
            .regionCount = 1,
            .pRegions = &region,
        };

        vkCmdCopyImageToBuffer2(m_command_buffer, &copy_image_to_buffer_info);

        HE_TRACE(
            "Copying {} texture to {} buffer",
            src->label().empty() ? "a" : fmt::format("the '{}'", src->label()),
            dst->label().empty() ? "a" : fmt::format("the '{}'", dst->label()));
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

        const VkImageCopy2 region = {
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
            .pRegions = &region,
        };

        vkCmdCopyImage2(m_command_buffer, &copy_image_info);

        HE_TRACE(
            "Copying {} texture to {} texture",
            src->label().empty() ? "a" : fmt::format("the '{}'", src->label()),
            dst->label().empty() ? "a" : fmt::format("the '{}'", dst->label()));
    }

    void VulkanCommandList::write_buffer(const std::shared_ptr<Buffer> &buffer, const void *data, const size_t size, const uint64_t offset)
    {
        const auto vulkan_buffer = std::dynamic_pointer_cast<VulkanBuffer>(buffer);

        if (buffer->byte_size() <= 65535)
        {
            vkCmdUpdateBuffer(m_command_buffer, vulkan_buffer->buffer(), offset, size, data);
        }
        else
        {
            const std::shared_ptr<Buffer> staging_buffer = m_graphics_device.create_staging_buffer({
                .label = fmt::format("{} Staging", buffer->label()),
                .byte_size = static_cast<uint64_t>(size),
                .usage = BufferUsage::Storage,
            });

            const auto vulkan_staging_buffer = std::dynamic_pointer_cast<VulkanBuffer>(staging_buffer);

            void *mapped_ptr = nullptr;
            vmaMapMemory(m_graphics_device.allocator(), vulkan_staging_buffer->allocation(), &mapped_ptr);
            memcpy(mapped_ptr, data, size);
            vmaUnmapMemory(m_graphics_device.allocator(), vulkan_staging_buffer->allocation());

            const VkBufferCopy2 region = {
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
                .pRegions = &region,
            };

            vkCmdCopyBuffer2(m_command_buffer, &copy_buffer_info);
        }

        HE_TRACE("Writing {} bytes to {} buffer", size, buffer->label().empty() ? "a" : fmt::format("the '{}'", buffer->label()));
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
        const auto vulkan_texture = std::dynamic_pointer_cast<VulkanTexture>(texture);

        const std::shared_ptr<Buffer> staging_buffer = m_graphics_device.create_staging_buffer({
            .label = fmt::format("{} Staging", texture->label()),
            .byte_size = static_cast<uint64_t>(data_size),
            .usage = BufferUsage::Storage,
        });

        const auto vulkan_staging_buffer = std::dynamic_pointer_cast<VulkanBuffer>(staging_buffer);

        void *mapped_ptr = nullptr;
        vmaMapMemory(m_graphics_device.allocator(), vulkan_staging_buffer->allocation(), &mapped_ptr);
        memcpy(mapped_ptr, data, data_size);
        vmaUnmapMemory(m_graphics_device.allocator(), vulkan_staging_buffer->allocation());

        const VkImageSubresourceLayers subresource_layers = {
            .aspectMask = VulkanTextureView::get_image_aspect_flags(texture->format()),
            .mipLevel = mip_level,
            .baseArrayLayer = array_index,
            .layerCount = 1,
        };

        const VkBufferImageCopy2 region = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2,
            .pNext = nullptr,
            .bufferOffset = data_offset,
            .bufferRowLength = 0,
            .bufferImageHeight = 0,
            .imageSubresource = subresource_layers,
            .imageOffset =
                VkOffset3D{
                    .x = offset.x,
                    .y = offset.y,
                    .z = offset.z,
                },
            .imageExtent =
                VkExtent3D{
                    .width = extent.width,
                    .height = extent.height,
                    .depth = extent.depth,
                },
        };

        const VkCopyBufferToImageInfo2 copy_buffer_to_image_info = {
            .sType = VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2,
            .pNext = nullptr,
            .srcBuffer = vulkan_staging_buffer->buffer(),
            .dstImage = vulkan_texture->image(),
            .dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            .regionCount = 1,
            .pRegions = &region,
        };

        vkCmdCopyBufferToImage2(m_command_buffer, &copy_buffer_to_image_info);

        HE_TRACE("Writing {} bytes to {} texture", data_size, texture->label().empty() ? "a" : fmt::format("the '{}'", texture->label()));
    }

    std::shared_ptr<ComputePass> VulkanCommandList::begin_compute_pass(const ComputePassDescriptor &descriptor) const
    {
        return std::make_shared<VulkanComputePass>(m_graphics_device, m_command_buffer, descriptor);
    }

    std::shared_ptr<RenderPass> VulkanCommandList::begin_render_pass(const RenderPassDescriptor &descriptor) const
    {
        return std::make_shared<VulkanRenderPass>(m_graphics_device, m_command_buffer, descriptor);
    }

    VkCommandBuffer VulkanCommandList::command_buffer() const
    {
        return m_command_buffer;
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

    VkAccessFlags2 VulkanCommandList::get_access_flags(const BarrierAccess barrier_access)
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
} // namespace he::rhi