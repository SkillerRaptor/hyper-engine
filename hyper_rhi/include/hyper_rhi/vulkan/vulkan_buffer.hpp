/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/buffer.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

#include <vk_mem_alloc.h>

namespace hyper_engine
{
    class VulkanGraphicsDevice;

    class VulkanBuffer final : public IBuffer
    {
    public:
        VulkanBuffer(VulkanGraphicsDevice &graphics_device, const BufferDescriptor &descriptor, bool staging = false);
        ~VulkanBuffer() override;

        std::string_view label() const override;

        uint64_t byte_size() const override;
        BufferUsage usage() const override;

        ResourceHandle handle() const override;

        VkBuffer buffer() const;
        VmaAllocation allocation() const;

        static VkBufferUsageFlags get_buffer_usage_flags(BufferUsage buffer_usage_flags);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        std::string m_label;

        uint64_t m_byte_size;
        BufferUsage m_usage;

        ResourceHandle m_handle;

        VkBuffer m_buffer;
        VmaAllocation m_allocation;
    };
} // namespace hyper_engine