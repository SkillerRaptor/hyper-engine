/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/sampler.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_engine
{
    class VulkanGraphicsDevice;

    class VulkanSampler final : public ISampler
    {
    public:
        VulkanSampler(VulkanGraphicsDevice &graphics_device, const SamplerDescriptor &descriptor);
        ~VulkanSampler() override;

        std::string_view label() const override;

        Filter mag_filter() const override;
        Filter min_filter() const override;
        Filter mipmap_filter() const override;
        AddressMode address_mode_u() const override;
        AddressMode address_mode_v() const override;
        AddressMode address_mode_w() const override;
        float mip_lod_bias() const override;
        CompareOperation compare_operation() const override;
        float min_lod() const override;
        float max_lod() const override;
        BorderColor border_color() const override;

        ResourceHandle handle() const override;

        VkSampler sampler() const;

        static VkFilter get_filter(Filter filter);
        static VkSamplerMipmapMode get_sampler_mipmap_mode(Filter filter);
        static VkSamplerAddressMode get_sampler_address_mode(AddressMode filter);
        static VkBorderColor get_border_color(BorderColor border_color);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        std::string m_label;

        Filter m_mag_filter;
        Filter m_min_filter;
        Filter m_mipmap_filter;
        AddressMode m_address_mode_u;
        AddressMode m_address_mode_v;
        AddressMode m_address_mode_w;
        float m_mip_lod_bias;
        CompareOperation m_compare_operation;
        float m_min_lod;
        float m_max_lod;
        BorderColor m_border_color;

        ResourceHandle m_handle;

        VkSampler m_sampler;
    };
} // namespace hyper_engine