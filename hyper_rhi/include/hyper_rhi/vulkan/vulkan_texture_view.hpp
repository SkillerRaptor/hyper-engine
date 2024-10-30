/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/texture.hpp"
#include "hyper_rhi/texture_view.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanTextureView final : public TextureView
    {
    public:
        VulkanTextureView(VulkanGraphicsDevice &graphics_device, const TextureViewDescriptor &descriptor);

        [[nodiscard]] VkImageView image_view() const;

    private:
        static VkImageAspectFlags get_image_aspect_flags(TextureFormat format);
        static VkImageViewType get_image_view_type(TextureDimension dimension);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkImageView m_image_view;
    };
} // namespace hyper_rhi