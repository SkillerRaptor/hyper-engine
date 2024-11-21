/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/texture.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

#include <vk_mem_alloc.h>

namespace he::rhi
{
    class VulkanGraphicsDevice;

    class VulkanTexture final : public ITexture
    {
    public:
        VulkanTexture(VulkanGraphicsDevice &graphics_device, const TextureDescriptor &descriptor, VkImage image = VK_NULL_HANDLE);
        ~VulkanTexture() override;

        VkImage image() const;
        VmaAllocation allocation() const;

        static Format format_to_texture_format(VkFormat format);
        static VkFormat get_format(Format format);
        static VkImageType get_image_type(Dimension dimension);
        static VkImageUsageFlags get_image_usage_flags(TextureUsage texture_usage_flags, Format format);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkImage m_image;
        VmaAllocation m_allocation;
    };
} // namespace he::rhi