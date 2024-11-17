/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_buffer.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_core/logger.hpp>

#include "hyper_rhi/vulkan/vulkan_descriptor_manager.hpp"
#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace he::rhi
{
    VulkanBuffer::VulkanBuffer(VulkanGraphicsDevice &graphics_device, const BufferDescriptor &descriptor, const bool staging)
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
        VmaAllocationCreateFlags allocation_flags = 0;
        if (staging)
        {
            allocation_flags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
            allocation_flags |= VMA_ALLOCATION_CREATE_MAPPED_BIT;
        }

        const VmaAllocationCreateInfo allocation_create_info = {
            .flags = allocation_flags,
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

        if ((descriptor.usage & BufferUsage::ShaderResource) == BufferUsage::ShaderResource)
        {
            if (m_handle.valid())
            {
                m_graphics_device.descriptor_manager().set_buffer(m_buffer, m_handle.handle());
            }
            else
            {
                m_handle = m_graphics_device.descriptor_manager().allocate_buffer_handle(m_buffer);
            }
        }

        // TODO: Add more trace information
        HE_TRACE("Created Buffer");
    }

    VulkanBuffer::~VulkanBuffer()
    {
        m_graphics_device.resource_queue().buffers.emplace_back(m_buffer, m_allocation, m_handle);
    }

    VkBuffer VulkanBuffer::buffer() const
    {
        return m_buffer;
    }

    VmaAllocation VulkanBuffer::allocation() const
    {
        return m_allocation;
    }

    VkBufferUsageFlags VulkanBuffer::get_buffer_usage_flags(const BufferUsage buffer_usage_flags)
    {
        VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        if ((buffer_usage_flags & BufferUsage::Index) == BufferUsage::Index)
        {
            usage_flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        }

        if ((buffer_usage_flags & BufferUsage::Indirect) == BufferUsage::Indirect)
        {
            usage_flags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
        }

        if ((buffer_usage_flags & BufferUsage::Storage) == BufferUsage::Storage)
        {
            usage_flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        }

        return usage_flags;
    }
} // namespace he::rhi