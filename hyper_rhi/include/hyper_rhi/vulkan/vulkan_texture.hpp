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
        VulkanTexture(VulkanGraphicsDevice &graphics_device, const TextureDescriptor &descriptor);
        VulkanTexture(VulkanGraphicsDevice &graphics_device, VkImage image, const TextureDescriptor &descriptor);
        ~VulkanTexture() override;

        [[nodiscard]] VkImage image() const;
        [[nodiscard]] VkImageView view() const;

        static TextureFormat format_to_texture_format(VkFormat format);
        static VkFormat texture_format_to_format(TextureFormat format);
        static VkImageType texture_dimension_to_image_type(TextureDimension dimension);
        static VkImageViewType texture_dimension_to_image_view_type(TextureDimension dimension);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkImage m_image;
        VmaAllocation m_allocation;
        VkImageView m_view;
    };
} // namespace hyper_rhi