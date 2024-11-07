/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/texture.hpp"
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

        [[nodiscard]] static Format format_to_texture_format(VkFormat format);
        [[nodiscard]] static VkFormat get_format(Format format);
        [[nodiscard]] static VkImageType get_image_type(TextureDimension dimension);
        [[nodiscard]] static VkImageUsageFlags get_image_usage_flags(TextureUsage texture_usage_flags, Format format);

        [[nodiscard]] HE_FORCE_INLINE VkImage image() const
        {
            return m_image;
        }

        [[nodiscard]] HE_FORCE_INLINE VmaAllocation allocation() const
        {
            return m_allocation;
        }

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkImage m_image;
        VmaAllocation m_allocation;
    };
} // namespace hyper_rhi