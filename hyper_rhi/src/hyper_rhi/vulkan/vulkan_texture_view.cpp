/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_texture_view.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_core/logger.hpp>

#include "hyper_rhi/vulkan/vulkan_descriptor_manager.hpp"
#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"
#include "hyper_rhi/vulkan/vulkan_texture.hpp"

namespace he::rhi
{
    VulkanTextureView::VulkanTextureView(VulkanGraphicsDevice &graphics_device, const TextureViewDescriptor &descriptor)
        : ITextureView(descriptor)
        , m_graphics_device(graphics_device)
        , m_image_view(VK_NULL_HANDLE)
    {
        const auto &texture = std::dynamic_pointer_cast<VulkanTexture>(m_texture);

        const VkImageViewType view_type = VulkanTextureView::get_image_view_type(texture->dimension());

        const VkFormat format = VulkanTexture::get_format(texture->format());

        const VkComponentSwizzle r = VulkanTextureView::get_component_swizzle(m_component_mapping.r);
        const VkComponentSwizzle g = VulkanTextureView::get_component_swizzle(m_component_mapping.g);
        const VkComponentSwizzle b = VulkanTextureView::get_component_swizzle(m_component_mapping.b);
        const VkComponentSwizzle a = VulkanTextureView::get_component_swizzle(m_component_mapping.a);
        const VkComponentMapping component_mapping = {
            .r = r,
            .g = g,
            .b = b,
            .a = a,
        };

        const VkImageAspectFlags aspect_mask = VulkanTextureView::get_image_aspect_flags(texture->format());
        const VkImageSubresourceRange subresource_range = {
            .aspectMask = aspect_mask,
            .baseMipLevel = m_subresource_range.base_mip_level,
            .levelCount = m_subresource_range.mip_level_count,
            .baseArrayLayer = m_subresource_range.base_array_level,
            .layerCount = m_subresource_range.array_layer_count,
        };

        const VkImageViewCreateInfo image_view_create_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = texture->image(),
            .viewType = view_type,
            .format = format,
            .components = component_mapping,
            .subresourceRange = subresource_range,
        };

        HE_VK_CHECK(vkCreateImageView(m_graphics_device.device(), &image_view_create_info, nullptr, &m_image_view));
        HE_ASSERT(m_image_view != VK_NULL_HANDLE);

        m_graphics_device.set_object_name(m_image_view, ObjectType::ImageView, m_label);

        if ((m_texture->usage() & TextureUsage::ShaderResource) == TextureUsage::ShaderResource)
        {
            if (m_handle.valid())
            {
                if ((m_texture->usage() & TextureUsage::Storage) == TextureUsage::Storage)
                {
                    m_graphics_device.descriptor_manager().set_storage_image(m_image_view, VK_IMAGE_LAYOUT_GENERAL, m_handle.handle());
                }
                else
                {
                    m_graphics_device.descriptor_manager().set_sampled_image(m_image_view, VK_IMAGE_LAYOUT_GENERAL, m_handle.handle());
                }
            }
            else
            {
                if ((m_texture->usage() & TextureUsage::Storage) == TextureUsage::Storage)
                {
                    m_handle = m_graphics_device.descriptor_manager().allocate_storage_image_handle(m_image_view, VK_IMAGE_LAYOUT_GENERAL);
                }
                else
                {
                    m_handle = m_graphics_device.descriptor_manager().allocate_sampled_image_handle(m_image_view, VK_IMAGE_LAYOUT_GENERAL);
                }
            }
        }

        // TODO: Add more trace information
        HE_TRACE("Created Image View");
    }

    VulkanTextureView::~VulkanTextureView()
    {
        m_graphics_device.resource_queue().texture_views.emplace_back(m_image_view, m_handle);
    }

    VkImageViewType VulkanTextureView::get_image_view_type(const Dimension dimension)
    {
        switch (dimension)
        {
        case Dimension::Texture1D:
            return VK_IMAGE_VIEW_TYPE_1D;
        case Dimension::Texture1DArray:
            return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        case Dimension::Texture2D:
            return VK_IMAGE_VIEW_TYPE_2D;
        case Dimension::Texture2DArray:
            return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        case Dimension::Texture3D:
            return VK_IMAGE_VIEW_TYPE_3D;
        case Dimension::Unknown:
        default:
            HE_UNREACHABLE();
        }
    }

    VkComponentSwizzle VulkanTextureView::get_component_swizzle(const ComponentSwizzle component_swizzle)
    {
        switch (component_swizzle)
        {
        case ComponentSwizzle::Identity:
            return VK_COMPONENT_SWIZZLE_IDENTITY;
        case ComponentSwizzle::Zero:
            return VK_COMPONENT_SWIZZLE_ZERO;
        case ComponentSwizzle::One:
            return VK_COMPONENT_SWIZZLE_ONE;
        case ComponentSwizzle::R:
            return VK_COMPONENT_SWIZZLE_R;
        case ComponentSwizzle::G:
            return VK_COMPONENT_SWIZZLE_G;
        case ComponentSwizzle::B:
            return VK_COMPONENT_SWIZZLE_B;
        case ComponentSwizzle::A:
            return VK_COMPONENT_SWIZZLE_A;
        default:
            HE_UNREACHABLE();
        }
    }

    VkImageAspectFlags VulkanTextureView::get_image_aspect_flags(const Format format)
    {
        switch (format)
        {
        case Format::R8Unorm:
        case Format::R8Snorm:
        case Format::R8Uint:
        case Format::R8Sint:
        case Format::R8Srgb:
        case Format::Rg8Unorm:
        case Format::Rg8Snorm:
        case Format::Rg8Uint:
        case Format::Rg8Sint:
        case Format::Rg8Srgb:
        case Format::Rgb8Unorm:
        case Format::Rgb8Snorm:
        case Format::Rgb8Uint:
        case Format::Rgb8Sint:
        case Format::Rgb8Srgb:
        case Format::Bgr8Unorm:
        case Format::Bgr8Snorm:
        case Format::Bgr8Uint:
        case Format::Bgr8Sint:
        case Format::Bgr8Srgb:
        case Format::Rgba8Unorm:
        case Format::Rgba8Snorm:
        case Format::Rgba8Uint:
        case Format::Rgba8Sint:
        case Format::Rgba8Srgb:
        case Format::Bgra8Unorm:
        case Format::Bgra8Snorm:
        case Format::Bgra8Uint:
        case Format::Bgra8Sint:
        case Format::Bgra8Srgb:
        case Format::R16Unorm:
        case Format::R16Snorm:
        case Format::R16Uint:
        case Format::R16Sint:
        case Format::R16Sfloat:
        case Format::Rg16Unorm:
        case Format::Rg16Snorm:
        case Format::Rg16Uint:
        case Format::Rg16Sint:
        case Format::Rg16Sfloat:
        case Format::Rgb16Unorm:
        case Format::Rgb16Snorm:
        case Format::Rgb16Uint:
        case Format::Rgb16Sint:
        case Format::Rgb16Sfloat:
        case Format::Rgba16Unorm:
        case Format::Rgba16Snorm:
        case Format::Rgba16Uint:
        case Format::Rgba16Sint:
        case Format::Rgba16Sfloat:
        case Format::R32Uint:
        case Format::R32Sint:
        case Format::R32Sfloat:
        case Format::Rg32Uint:
        case Format::Rg32Sint:
        case Format::Rg32Sfloat:
        case Format::Rgb32Uint:
        case Format::Rgb32Sint:
        case Format::Rgb32Sfloat:
        case Format::Rgba32Uint:
        case Format::Rgba32Sint:
        case Format::Rgba32Sfloat:
        case Format::R64Uint:
        case Format::R64Sint:
        case Format::R64Sfloat:
        case Format::Rg64Uint:
        case Format::Rg64Sint:
        case Format::Rg64Sfloat:
        case Format::Rgb64Uint:
        case Format::Rgb64Sint:
        case Format::Rgb64Sfloat:
        case Format::Rgba64Uint:
        case Format::Rgba64Sint:
        case Format::Rgba64Sfloat:
            return VK_IMAGE_ASPECT_COLOR_BIT;
        case Format::D16Unorm:
        case Format::D32Sfloat:
            return VK_IMAGE_ASPECT_DEPTH_BIT;
        case Format::S8Uint:
            return VK_IMAGE_ASPECT_STENCIL_BIT;
        case Format::D16UnormS8Uint:
        case Format::D24UnormS8Uint:
        case Format::D32SfloatS8Uint:
            return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        case Format::Unknown:
        default:
            HE_UNREACHABLE();
        }
    }

    VkImageView VulkanTextureView::image_view() const
    {
        return m_image_view;
    }
} // namespace he::rhi