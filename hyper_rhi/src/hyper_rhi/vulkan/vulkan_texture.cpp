/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_texture.hpp"

#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"
#include "hyper_rhi/vulkan/vulkan_texture_view.hpp"

namespace hyper_rhi
{
    VulkanTexture::VulkanTexture(VulkanGraphicsDevice &graphics_device, const TextureDescriptor &descriptor, const VkImage image)
        : Texture(descriptor)
        , m_graphics_device(graphics_device)
        , m_image(image)
        , m_allocation(VK_NULL_HANDLE)
    {
        if (m_image == VK_NULL_HANDLE)
        {
            const VkImageType image_type = VulkanTexture::get_image_type(descriptor.dimension);
            const VkFormat format = VulkanTexture::get_format(descriptor.format);
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
                .samples = VK_SAMPLE_COUNT_1_BIT,
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

            m_graphics_device.set_object_name(m_image, ObjectType::Image, m_label);

            HE_TRACE("Created Image {}", m_label.empty() ? "" : fmt::format("'{}'", m_label));
        }

        m_view = m_graphics_device.create_texture_view({
            .label = m_label,
            .base_mip_level = 0,
            .mip_level_count = 1,
            .base_array_level = 0,
            .array_layer_count = 1,
            .component_mapping = {
                .r = TextureComponentSwizzle::Identity,
                .g = TextureComponentSwizzle::Identity,
                .b = TextureComponentSwizzle::Identity,
                .a = TextureComponentSwizzle::Identity,
            },
            .format = m_format,
            .dimension = m_dimension,
            .texture = *this,
        });
    }

    VulkanTexture::~VulkanTexture()
    {
        const auto view = std::dynamic_pointer_cast<VulkanTextureView>(m_view);
        m_graphics_device.resource_queue().textures.emplace_back(m_image, m_allocation, view->image_view());
    }

    VkImage VulkanTexture::image() const
    {
        return m_image;
    }

    VmaAllocation VulkanTexture::allocation() const
    {
        return m_allocation;
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

    VkImageUsageFlags VulkanTexture::get_image_usage_flags(const TextureUsage texture_usage_flags, const TextureFormat format)
    {
        VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        if ((texture_usage_flags & TextureUsage::ShaderResource) == TextureUsage::ShaderResource)
        {
            usage |= VK_IMAGE_USAGE_STORAGE_BIT;
        }

        if ((texture_usage_flags & TextureUsage::RenderTarget) == TextureUsage::RenderTarget)
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
} // namespace hyper_rhi