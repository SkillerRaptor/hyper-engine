/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/buffer.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

#include <vk_mem_alloc.h>

namespace he::rhi
{
    class VulkanGraphicsDevice;

    class VulkanBuffer final : public IBuffer
    {
    public:
        VulkanBuffer(VulkanGraphicsDevice &graphics_device, const BufferDescriptor &descriptor, bool staging = false);
        ~VulkanBuffer() override;

        VkBuffer buffer() const;
        VmaAllocation allocation() const;

        static VkBufferUsageFlags get_buffer_usage_flags(BufferUsage buffer_usage_flags);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkBuffer m_buffer;
        VmaAllocation m_allocation;
    };
} // namespace he::rhi