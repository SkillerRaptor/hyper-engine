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
        ~VulkanTextureView() override;

        [[nodiscard]] static VkImageAspectFlags get_image_aspect_flags(Format format);
        [[nodiscard]] static VkComponentSwizzle get_component_swizzle(TextureComponentSwizzle component_swizzle);
        [[nodiscard]] static VkImageViewType get_image_view_type(TextureDimension dimension);

        [[nodiscard]] HE_FORCE_INLINE VkImageView image_view() const
        {
            return m_image_view;
        }

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkImageView m_image_view;
    };
} // namespace hyper_rhi