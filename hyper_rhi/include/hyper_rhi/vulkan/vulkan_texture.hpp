/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/texture.hpp"
#include "hyper_rhi/texture_view.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"
#include "hyper_rhi/vulkan/vulkan_texture_view.hpp"

#include <vk_mem_alloc.h>

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanTexture final : public Texture
    {
    public:
        VulkanTexture(VulkanGraphicsDevice &graphics_device, const TextureDescriptor &descriptor, VkImage image = VK_NULL_HANDLE);
        ~VulkanTexture() override;

        void set_view(TextureView &view);

        [[nodiscard]] VkImage image() const;
        [[nodiscard]] VmaAllocation allocation() const;

        static TextureFormat format_to_texture_format(VkFormat format);
        static VkFormat get_format(TextureFormat format);

    private:
        static VkImageType get_image_type(TextureDimension dimension);
        static VkImageUsageFlags get_image_usage_flags(TextureUsage texture_usage_flags, TextureFormat format);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkImage m_image;
        VmaAllocation m_allocation;
    };
} // namespace hyper_rhi