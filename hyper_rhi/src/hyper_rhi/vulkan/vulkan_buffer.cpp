/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_buffer.hpp"

#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace hyper_rhi
{
    VulkanBuffer::VulkanBuffer(VulkanGraphicsDevice &graphics_device, const BufferDescriptor &descriptor)
        : Buffer(descriptor)
        , m_graphics_device(graphics_device)
        , m_buffer(VK_NULL_HANDLE)
        , m_allocation(VK_NULL_HANDLE)
    {
        VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        if (descriptor.is_constant_buffer)
        {
            usage_flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        }

        if (descriptor.is_index_buffer)
        {
            usage_flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        }

        uint64_t size = descriptor.byte_size;
        if (size < 65536)
        {
            // NOTE: Make buffer larger to be a multiple of 4 if the size is less than 64kb
            size = (size + 3) & ~3ull;
        }

        // TODO: Make the size aligned for small uploads
        const VkBufferCreateInfo buffer_create_info = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = descriptor.byte_size,
            .usage = usage_flags,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices = nullptr,
        };

        // TODO: Add more explicit allocation
        constexpr VmaAllocationCreateInfo allocation_create_info = {
            .flags = 0,
            .usage = VMA_MEMORY_USAGE_AUTO,
            .requiredFlags = 0,
            .preferredFlags = 0,
            .memoryTypeBits = 0,
            .pool = VK_NULL_HANDLE,
            .pUserData = nullptr,
            .priority = 0,
        };

        HE_VK_CHECK(
            vmaCreateBuffer(m_graphics_device.allocator(), &buffer_create_info, &allocation_create_info, &m_buffer, &m_allocation, nullptr));
        HE_ASSERT(m_buffer != VK_NULL_HANDLE);
        HE_ASSERT(m_allocation != VK_NULL_HANDLE);

        if (descriptor.is_constant_buffer && !descriptor.has_index)
        {
            m_handle = m_graphics_device.descriptor_manager().allocate_buffer_handle(m_buffer);
        }

        if (!m_label.empty())
        {
            m_graphics_device.set_object_name(m_buffer, VK_OBJECT_TYPE_BUFFER, m_label);
        }

        HE_TRACE("Created Buffer {} with {} bytes", m_label.empty() ? "" : fmt::format("'{}'", m_label), descriptor.byte_size);
    }

    VulkanBuffer::~VulkanBuffer()
    {
        m_graphics_device.resource_queue().buffers.emplace_back(m_buffer, m_allocation, m_handle);
    }

    VkBuffer VulkanBuffer::buffer() const
    {
        return m_buffer;
    }
} // namespace hyper_rhi