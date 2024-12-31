/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/texture.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

#include <vk_mem_alloc.h>

namespace hyper_engine
{
    class VulkanGraphicsDevice;

    class VulkanTexture final : public Texture
    {
    public:
        VulkanTexture(VulkanGraphicsDevice &graphics_device, const TextureDescriptor &descriptor, VkImage image = VK_NULL_HANDLE);
        ~VulkanTexture() override;

        std::string_view label() const override;

        uint32_t width() const override;
        uint32_t height() const override;
        uint32_t depth() const override;
        uint32_t array_size() const override;
        uint32_t mip_levels() const override;
        Format format() const override;
        Dimension dimension() const override;
        TextureUsage usage() const override;

        VkImage image() const;
        VmaAllocation allocation() const;

        static Format format_to_texture_format(VkFormat format);
        static VkFormat get_format(Format format);
        static VkImageType get_image_type(Dimension dimension);
        static VkImageUsageFlags get_image_usage_flags(TextureUsage texture_usage_flags, Format format);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        std::string m_label;

        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_depth;
        uint32_t m_array_size;
        uint32_t m_mip_levels;
        Format m_format;
        Dimension m_dimension;
        TextureUsage m_usage;

        VkImage m_image;
        VmaAllocation m_allocation;
    };
} // namespace hyper_engine