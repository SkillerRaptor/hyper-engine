/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/dimension.hpp"
#include "hyper_rhi/format.hpp"
#include "hyper_rhi/texture_view.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace he::rhi
{
    class VulkanGraphicsDevice;

    class VulkanTextureView final : public ITextureView
    {
    public:
        VulkanTextureView(VulkanGraphicsDevice &graphics_device, const TextureViewDescriptor &descriptor);
        ~VulkanTextureView() override;

        VkImageView image_view() const;

        static VkImageAspectFlags get_image_aspect_flags(Format format);
        static VkComponentSwizzle get_component_swizzle(ComponentSwizzle component_swizzle);
        static VkImageViewType get_image_view_type(Dimension dimension);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkImageView m_image_view;
    };
} // namespace he::rhi