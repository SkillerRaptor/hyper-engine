/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_texture.hpp"

#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace hyper_rhi
{
    VulkanTexture::VulkanTexture(VulkanGraphicsDevice &graphics_device, const TextureDescriptor &descriptor)
        : Texture(descriptor)
        , m_graphics_device(graphics_device)
        , m_image(VK_NULL_HANDLE)
        , m_allocation(VK_NULL_HANDLE)
        , m_view(VK_NULL_HANDLE)
    {
    }

    VulkanTexture::VulkanTexture(VulkanGraphicsDevice &graphics_device, const VkImage image, const TextureDescriptor &descriptor)
        : Texture(descriptor)
        , m_graphics_device(graphics_device)
        , m_image(image)
        , m_allocation(VK_NULL_HANDLE)
        , m_view(VK_NULL_HANDLE)
    {
        constexpr VkComponentMapping component_mapping = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        };

        constexpr VkImageSubresourceRange subresource_range = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        };

        const VkImageViewCreateInfo image_view_create_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = m_image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = VulkanTexture::texture_format_to_format(descriptor.format),
            .components = component_mapping,
            .subresourceRange = subresource_range,
        };

        HE_VK_CHECK(vkCreateImageView(m_graphics_device.device(), &image_view_create_info, nullptr, &m_view));
        HE_ASSERT(m_view != VK_NULL_HANDLE);

        HE_TRACE("Created Image View {} with a size of {}x{}", m_label.empty() ? "" : fmt::format("'{}'", m_label), m_width, m_height);
    }

    VulkanTexture::~VulkanTexture()
    {
        m_graphics_device.resource_queue().textures.emplace_back(m_image, m_allocation, m_view, m_handle);
    }

    VkImage VulkanTexture::image() const
    {
        return m_image;
    }

    VkImageView VulkanTexture::view() const
    {
        return m_view;
    }

    TextureFormat VulkanTexture::format_to_texture_format(const VkFormat format)
    {
        switch (format)
        {
        case VK_FORMAT_B8G8R8A8_SRGB:
            return TextureFormat::B8G8R8A8_Srgb;
        default:
            HE_UNREACHABLE();
        }
    }

    VkFormat VulkanTexture::texture_format_to_format(const TextureFormat format)
    {
        switch (format)
        {
        case TextureFormat::Unknown:
            return VK_FORMAT_UNDEFINED;
        case TextureFormat::B8G8R8A8_Srgb:
            return VK_FORMAT_B8G8R8A8_SRGB;
        default:
            HE_UNREACHABLE();
        }
    }

    VkImageType VulkanTexture::texture_dimension_to_image_type(const TextureDimension dimension)
    {
        switch (dimension)
        {
        case TextureDimension::Texture1D:
        case TextureDimension::Texture1DArray:
            return VK_IMAGE_TYPE_1D;
        case TextureDimension::Texture2D:
        case TextureDimension::Texture2DArray:
            return VK_IMAGE_TYPE_2D;
        case TextureDimension::Texture3D:
            return VK_IMAGE_TYPE_3D;
        case TextureDimension::Unknown:
        default:
            HE_UNREACHABLE();
        }
    }

    VkImageViewType VulkanTexture::texture_dimension_to_image_view_type(const TextureDimension dimension)
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