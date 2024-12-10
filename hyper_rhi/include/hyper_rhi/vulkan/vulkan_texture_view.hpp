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

namespace hyper_engine
{
    class VulkanGraphicsDevice;

    class VulkanTextureView final : public TextureView
    {
    public:
        VulkanTextureView(VulkanGraphicsDevice &graphics_device, const TextureViewDescriptor &descriptor);
        ~VulkanTextureView() override;

        std::string_view label() const override;

        const std::shared_ptr<Texture> &texture() const override;
        SubresourceRange subresource_range() const override;
        ComponentMapping component_mapping() const override;

        ResourceHandle handle() const override;

        VkImageView image_view() const;

        static VkImageAspectFlags get_image_aspect_flags(Format format);
        static VkComponentSwizzle get_component_swizzle(ComponentSwizzle component_swizzle);
        static VkImageViewType get_image_view_type(Dimension dimension);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        std::string m_label;

        std::shared_ptr<Texture> m_texture;
        SubresourceRange m_subresource_range;
        ComponentMapping m_component_mapping;

        ResourceHandle m_handle;

        VkImageView m_image_view;
    };
} // namespace hyper_engine