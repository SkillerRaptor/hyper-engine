/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_texture.hpp"

#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace hyper_rhi
{
    VulkanTexture::VulkanTexture(VulkanGraphicsDevice &graphics_device, const TextureDescriptor &descriptor, const VkImage image)
        : Texture(descriptor)
        , m_graphics_device(graphics_device)
        , m_image(image)
        , m_allocation(VK_NULL_HANDLE)
        , m_view(VK_NULL_HANDLE)
    {
        const VkFormat format = VulkanTexture::get_format(descriptor.format);
        if (m_image == VK_NULL_HANDLE)
        {
            const VkImageType image_type = VulkanTexture::get_image_type(descriptor.dimension);
            const VkSampleCountFlagBits samples = VulkanTexture::get_sample_count_flags(descriptor.sample_count);
            const VkImageUsageFlags usage = VulkanTexture::get_image_usage_flags(descriptor.usage, descriptor.format);

            const VkImageCreateInfo image_create_info = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .imageType = image_type,
                .format = format,
                .extent = {
                    .width = descriptor.width,
                    .height = descriptor.height,
                    .depth = descriptor.depth,
                },
                .mipLevels = descriptor.mip_levels,
                .arrayLayers = descriptor.array_size,
                .samples = samples,
                .tiling = VK_IMAGE_TILING_OPTIMAL,
                .usage = usage,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .queueFamilyIndexCount = 0,
                .pQueueFamilyIndices = nullptr,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            };

            constexpr VmaAllocationCreateInfo allocation_create_info = {
                .flags = 0,
                .usage = VMA_MEMORY_USAGE_AUTO,
                .requiredFlags = 0,
                .preferredFlags = 0,
                .memoryTypeBits = 0,
                .pool = VK_NULL_HANDLE,
                .pUserData = nullptr,
                .priority = 0,
            };

            HE_VK_CHECK(
                vmaCreateImage(m_graphics_device.allocator(), &image_create_info, &allocation_create_info, &m_image, &m_allocation, nullptr));
            HE_ASSERT(m_image != VK_NULL_HANDLE);
            HE_ASSERT(m_allocation != VK_NULL_HANDLE);

            HE_TRACE("Created Image {}", m_label.empty() ? "" : fmt::format("'{}'", m_label));
        }

        constexpr VkComponentMapping component_mapping = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        };

        const VkImageAspectFlags aspect_mask = VulkanTexture::get_image_aspect_flags(descriptor.format);
        const VkImageSubresourceRange subresource_range = {
            .aspectMask = aspect_mask,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        };

        const VkImageViewType view_type = VulkanTexture::get_image_view_type(descriptor.dimension);
        const VkImageViewCreateInfo image_view_create_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = m_image,
            .viewType = view_type,
            .format = format,
            .components = component_mapping,
            .subresourceRange = subresource_range,
        };

        HE_VK_CHECK(vkCreateImageView(m_graphics_device.device(), &image_view_create_info, nullptr, &m_view));
        HE_ASSERT(m_view != VK_NULL_HANDLE);

        if ((descriptor.usage & TextureUsageFlags::ShaderResource) == TextureUsageFlags::ShaderResource)
        {
            // TODO: Add resource handle
            if (!m_handle.is_valid())
            {
            }
            else
            {
            }
        }

        m_graphics_device.set_object_name(m_image, ObjectType::Image, m_label);
        m_graphics_device.set_object_name(m_view, ObjectType::ImageView, m_label);

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
        case VK_FORMAT_D32_SFLOAT:
            return TextureFormat::D32_SFloat;
        default:
            HE_UNREACHABLE();
        }
    }

    VkFormat VulkanTexture::get_format(const TextureFormat format)
    {
        switch (format)
        {
        case TextureFormat::Unknown:
            return VK_FORMAT_UNDEFINED;
        case TextureFormat::B8G8R8A8_Srgb:
            return VK_FORMAT_B8G8R8A8_SRGB;
        case TextureFormat::D32_SFloat:
            return VK_FORMAT_D32_SFLOAT;
        default:
            HE_UNREACHABLE();
        }
    }

    VkImageType VulkanTexture::get_image_type(const TextureDimension dimension)
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

    VkImageViewType VulkanTexture::get_image_view_type(const TextureDimension dimension)
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

    VkSampleCountFlagBits VulkanTexture::get_sample_count_flags(const uint32_t sample_count)
    {
        switch (sample_count)
        {
        case 1:
            return VK_SAMPLE_COUNT_1_BIT;
        case 2:
            return VK_SAMPLE_COUNT_2_BIT;
        case 4:
            return VK_SAMPLE_COUNT_4_BIT;
        case 8:
            return VK_SAMPLE_COUNT_8_BIT;
        case 16:
            return VK_SAMPLE_COUNT_16_BIT;
        case 32:
            return VK_SAMPLE_COUNT_32_BIT;
        case 64:
            return VK_SAMPLE_COUNT_64_BIT;
        default:
            HE_UNREACHABLE();
        }
    }

    VkImageUsageFlags VulkanTexture::get_image_usage_flags(const TextureUsageFlags texture_usage_flags, const TextureFormat format)
    {
        VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        if ((texture_usage_flags & TextureUsageFlags::ShaderResource) == TextureUsageFlags::ShaderResource)
        {
            usage |= VK_IMAGE_USAGE_STORAGE_BIT;
        }

        if ((texture_usage_flags & TextureUsageFlags::RenderTarget) == TextureUsageFlags::RenderTarget)
        {
            switch (format)
            {
            case TextureFormat::B8G8R8A8_Srgb:
                usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
                break;
            case TextureFormat::D32_SFloat:
                usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
                break;
            case TextureFormat::Unknown:
            default:
                HE_UNREACHABLE();
            }
        }

        return usage;
    }

    VkImageAspectFlags VulkanTexture::get_image_aspect_flags(TextureFormat format)
    {
        VkImageAspectFlags aspect_mask = VK_IMAGE_ASPECT_NONE;
        switch (format)
        {
        case TextureFormat::B8G8R8A8_Srgb:
            aspect_mask |= VK_IMAGE_ASPECT_COLOR_BIT;
            break;
        case TextureFormat::D32_SFloat:
            aspect_mask |= VK_IMAGE_ASPECT_DEPTH_BIT;
            break;
        case TextureFormat::Unknown:
        default:
            HE_UNREACHABLE();
        }

        return aspect_mask;
    }
} // namespace hyper_rhi