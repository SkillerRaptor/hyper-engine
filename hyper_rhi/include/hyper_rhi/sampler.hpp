/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

#include "hyper_rhi/compare_operation.hpp"
#include "hyper_rhi/resource_handle.hpp"

namespace hyper_engine
{
    enum class Filter : uint8_t
    {
        Nearest,
        Linear,
    };

    enum class AddressMode : uint8_t
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
        MirrorClampToEdge
    };

    enum class BorderColor : uint8_t
    {
        TransparentBlack,
        OpaqueBlack,
        OpaqueWhite,
    };

    struct SamplerDescriptor
    {
        std::string label;

        Filter mag_filter = Filter::Linear;
        Filter min_filter = Filter::Linear;
        Filter mipmap_filter = Filter::Linear;
        AddressMode address_mode_u = AddressMode::ClampToEdge;
        AddressMode address_mode_v = AddressMode::ClampToEdge;
        AddressMode address_mode_w = AddressMode::ClampToEdge;
        float mip_lod_bias = 0.0;
        CompareOperation compare_operation = CompareOperation::Less;
        float min_lod = 0.0;
        float max_lod = 1.0;
        BorderColor border_color = BorderColor::TransparentBlack;

        ResourceHandle handle;
    };

    class ISampler
    {
    public:
        virtual ~ISampler() = default;

        std::string_view label() const;

        Filter mag_filter() const;
        Filter min_filter() const;
        Filter mipmap_filter() const;
        AddressMode address_mode_u() const;
        AddressMode address_mode_v() const;
        AddressMode address_mode_w() const;
        float mip_lod_bias() const;
        CompareOperation compare_operation() const;
        float min_lod() const;
        float max_lod() const;
        BorderColor border_color() const;

        ResourceHandle handle() const;

    protected:
        explicit ISampler(const SamplerDescriptor &descriptor);

    protected:
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
    };
} // namespace hyper_engine