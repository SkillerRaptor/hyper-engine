/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/buffer.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

#include <vk_mem_alloc.h>

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanBuffer final : public Buffer
    {
    public:
        VulkanBuffer(VulkanGraphicsDevice &graphics_device, const BufferDescriptor &descriptor);
        ~VulkanBuffer() override;

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkBuffer m_buffer;
        VmaAllocation m_allocation;
    };
} // namespace hyper_rhi