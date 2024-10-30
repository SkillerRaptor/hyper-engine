/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_buffer.hpp"

#include <hyper_core/bitmask.hpp>

#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace hyper_rhi
{
    VulkanBuffer::VulkanBuffer(VulkanGraphicsDevice &graphics_device, const BufferDescriptor &descriptor)
        : Buffer(descriptor)
        , m_graphics_device(graphics_device)
        , m_buffer(VK_NULL_HANDLE)
        , m_allocation(VK_NULL_HANDLE)
    {
        const VkBufferUsageFlags usage_flags = VulkanBuffer::get_buffer_usage_flags(descriptor.usage);

        // NOTE: To be able to use vkCmdUpdateBuffer the buffer size needs to be a multiple of 4 if the size is less than 64kb
        if (m_byte_size < 65536)
        {
            m_byte_size = (m_byte_size + 3) & ~3ull;
        }

        const VkBufferCreateInfo buffer_create_info = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = m_byte_size,
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

        m_graphics_device.set_object_name(m_buffer, ObjectType::Buffer, m_label);

        if ((descriptor.usage & BufferUsageFlags::ShaderResource) == BufferUsageFlags::ShaderResource)
        {
            if (!m_handle.is_valid())
            {
                m_handle = m_graphics_device.descriptor_manager().allocate_buffer_handle(m_buffer);
            }
            else
            {
                m_graphics_device.descriptor_manager().set_dynamic_buffer(m_buffer, m_handle.handle());
            }
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

    VkBufferUsageFlags VulkanBuffer::get_buffer_usage_flags(const BufferUsageFlags buffer_usage_flags)
    {
        VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        if ((buffer_usage_flags & BufferUsageFlags::ShaderResource) == BufferUsageFlags::ShaderResource)
        {
            usage_flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        }

        if ((buffer_usage_flags & BufferUsageFlags::IndexBuffer) == BufferUsageFlags::IndexBuffer)
        {
            usage_flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        }

        return usage_flags;
    }
} // namespace hyper_rhi