/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/texture.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

#include <vk_mem_alloc.h>

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanTexture final : public Texture
    {
    public:
        VulkanTexture(VulkanGraphicsDevice &graphics_device, const TextureDescriptor &descriptor, VkImage image = VK_NULL_HANDLE);
        ~VulkanTexture() override;

        [[nodiscard]] VkImage image() const;
        [[nodiscard]] VkImageView view() const;

        static TextureFormat format_to_texture_format(VkFormat format);

    private:
        static VkFormat get_format(TextureFormat format);
        static VkImageType get_image_type(TextureDimension dimension);
        static VkImageViewType get_image_view_type(TextureDimension dimension);
        static VkSampleCountFlagBits get_sample_count_flags(uint32_t sample_count);
        static VkImageUsageFlags get_image_usage_flags(TextureUsageFlags texture_usage_flags, TextureFormat format);
        static VkImageAspectFlags get_image_aspect_flags(TextureFormat format);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkImage m_image;
        VmaAllocation m_allocation;
        VkImageView m_view;
    };
} // namespace hyper_rhi