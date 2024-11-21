/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_sampler.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_core/logger.hpp>

#include "hyper_rhi/vulkan/vulkan_descriptor_manager.hpp"
#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"
#include "hyper_rhi/vulkan/vulkan_render_pipeline.hpp"

namespace hyper_engine
{
    VulkanSampler::VulkanSampler(VulkanGraphicsDevice &graphics_device, const SamplerDescriptor &descriptor)
        : ISampler(descriptor)
        , m_graphics_device(graphics_device)
        , m_sampler(VK_NULL_HANDLE)
    {
        const VkFilter mag_filter = VulkanSampler::get_filter(descriptor.mag_filter);
        const VkFilter min_filter = VulkanSampler::get_filter(descriptor.min_filter);
        const VkSamplerMipmapMode mipmap_filter = VulkanSampler::get_sampler_mipmap_mode(descriptor.mipmap_filter);
        const VkSamplerAddressMode address_mode_u = VulkanSampler::get_sampler_address_mode(descriptor.address_mode_u);
        const VkSamplerAddressMode address_mode_v = VulkanSampler::get_sampler_address_mode(descriptor.address_mode_v);
        const VkSamplerAddressMode address_mode_w = VulkanSampler::get_sampler_address_mode(descriptor.address_mode_w);
        const VkCompareOp compare_operation = VulkanRenderPipeline::get_compare_operation(m_compare_operation);
        const VkBorderColor border_color = VulkanSampler::get_border_color(m_border_color);
        const VkSamplerCreateInfo sampler_create_info = {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .magFilter = mag_filter,
            .minFilter = min_filter,
            .mipmapMode = mipmap_filter,
            .addressModeU = address_mode_u,
            .addressModeV = address_mode_v,
            .addressModeW = address_mode_w,
            .mipLodBias = m_mip_lod_bias,
            .anisotropyEnable = VK_FALSE,
            .maxAnisotropy = 0.0,
            .compareEnable = VK_TRUE,
            .compareOp = compare_operation,
            .minLod = m_min_lod,
            .maxLod = m_max_lod,
            .borderColor = border_color,
            .unnormalizedCoordinates = VK_FALSE,
        };

        vkCreateSampler(m_graphics_device.device(), &sampler_create_info, nullptr, &m_sampler);

        m_graphics_device.set_object_name(m_sampler, ObjectType::Sampler, m_label);

        if (m_handle.valid())
        {
            m_graphics_device.descriptor_manager().set_sampler(m_sampler, m_handle.handle());
        }
        else
        {
            m_handle = m_graphics_device.descriptor_manager().allocate_sampler_handle(m_sampler);
        }

        // TODO: Add more trace information
        HE_TRACE("Created Sampler");
    }

    VulkanSampler::~VulkanSampler()
    {
        m_graphics_device.resource_queue().samplers.emplace_back(m_sampler);
    }

    VkSampler VulkanSampler::sampler() const
    {
        return m_sampler;
    }

    VkFilter VulkanSampler::get_filter(const Filter filter)
    {
        switch (filter)
        {
        case Filter::Nearest:
            return VK_FILTER_NEAREST;
        case Filter::Linear:
            return VK_FILTER_LINEAR;
        default:
            HE_UNREACHABLE();
        }
    }

    VkSamplerMipmapMode VulkanSampler::get_sampler_mipmap_mode(const Filter filter)
    {
        switch (filter)
        {
        case Filter::Nearest:
            return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case Filter::Linear:
            return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        default:
            HE_UNREACHABLE();
        }
    }

    VkSamplerAddressMode VulkanSampler::get_sampler_address_mode(const AddressMode filter)
    {
        switch (filter)
        {
        case AddressMode::Repeat:
            return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case AddressMode::MirroredRepeat:
            return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case AddressMode::ClampToEdge:
            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case AddressMode::ClampToBorder:
            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        case AddressMode::MirrorClampToEdge:
            return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
        default:
            HE_UNREACHABLE();
        }
    }

    VkBorderColor VulkanSampler::get_border_color(const BorderColor border_color)
    {
        switch (border_color)
        {
        case BorderColor::TransparentBlack:
            return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
        case BorderColor::OpaqueBlack:
            return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
        case BorderColor::OpaqueWhite:
            return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
        default:
            HE_UNREACHABLE();
        }
    }
} // namespace hyper_engine