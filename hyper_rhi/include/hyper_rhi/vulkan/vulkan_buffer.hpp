/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/buffer.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

#include <vk_mem_alloc.h>

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanBuffer final : public Buffer
    {
    public:
        VulkanBuffer(VulkanGraphicsDevice &graphics_device, const BufferDescriptor &descriptor, bool staging = false);
        ~VulkanBuffer() override;

        [[nodiscard]] static VkBufferUsageFlags get_buffer_usage_flags(BufferUsage buffer_usage_flags);

        [[nodiscard]] HE_FORCE_INLINE VkBuffer buffer() const
        {
            return m_buffer;
        }

        [[nodiscard]] HE_FORCE_INLINE VmaAllocation allocation() const
        {
            return m_allocation;
        }

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkBuffer m_buffer;
        VmaAllocation m_allocation;
    };
} // namespace hyper_rhi