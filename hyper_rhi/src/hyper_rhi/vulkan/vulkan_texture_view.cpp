/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_texture_view.hpp"

#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"
#include "hyper_rhi/vulkan/vulkan_texture.hpp"

namespace hyper_rhi
{
    VulkanTextureView::VulkanTextureView(VulkanGraphicsDevice &graphics_device, const TextureViewDescriptor &descriptor)
        : TextureView(descriptor)
        , m_graphics_device(graphics_device)
        , m_image_view(VK_NULL_HANDLE)
    {
        constexpr VkComponentMapping component_mapping = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        };

        const VkImageAspectFlags aspect_mask = VulkanTextureView::get_image_aspect_flags(descriptor.format);
        const VkImageSubresourceRange subresource_range = {
            .aspectMask = aspect_mask,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        };

        const auto &texture = dynamic_cast<VulkanTexture &>(descriptor.texture);
        const VkImageViewType view_type = VulkanTextureView::get_image_view_type(descriptor.dimension);
        const VkFormat format = VulkanTexture::get_format(descriptor.format);
        const VkImageViewCreateInfo image_view_create_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = texture.image(),
            .viewType = view_type,
            .format = format,
            .components = component_mapping,
            .subresourceRange = subresource_range,
        };

        HE_VK_CHECK(vkCreateImageView(m_graphics_device.device(), &image_view_create_info, nullptr, &m_image_view));
        HE_ASSERT(m_image_view != VK_NULL_HANDLE);

        m_graphics_device.set_object_name(m_image_view, ObjectType::ImageView, m_label);

        HE_TRACE("Created Image View {}", m_label.empty() ? "" : fmt::format("'{}'", m_label));
    }

    VkImageView VulkanTextureView::image_view() const
    {
        return m_image_view;
    }

    VkImageAspectFlags VulkanTextureView::get_image_aspect_flags(const TextureFormat format)
    {
        switch (format)
        {
        case TextureFormat::B8G8R8A8_Srgb:
            return VK_IMAGE_ASPECT_COLOR_BIT;
        case TextureFormat::D32_SFloat:
            return VK_IMAGE_ASPECT_DEPTH_BIT;
        case TextureFormat::Unknown:
        default:
            HE_UNREACHABLE();
        }
    }

    VkImageViewType VulkanTextureView::get_image_view_type(const TextureDimension dimension)
    {
        switch (dimension)
        {
        case TextureDimension::Texture1D:
            return VK_IMAGE_VIEW_TYPE_1D;
        case TextureDimension::Texture1DArray:
            return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        case TextureDimension::Texture2D:
            return VK_IMAGE_VIEW_TYPE_2D;
        case TextureDimension::Texture2DArray:
            return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        case TextureDimension::Texture3D:
            return VK_IMAGE_VIEW_TYPE_3D;
        case TextureDimension::Unknown:
        default:
            HE_UNREACHABLE();
        }
    }
} // namespace hyper_rhi