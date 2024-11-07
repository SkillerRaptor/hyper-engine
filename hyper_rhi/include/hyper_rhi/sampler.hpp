/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/render_pipeline.hpp"
#include "hyper_rhi/resource.hpp"
#include "hyper_rhi/resource_handle.hpp"

namespace hyper_rhi
{
    enum Filter : uint8_t
    {
        Nearest,
        Linear,
    };

    enum class SamplerAddressMode : uint8_t
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
        SamplerAddressMode address_mode_u = SamplerAddressMode::ClampToEdge;
        SamplerAddressMode address_mode_v = SamplerAddressMode::ClampToEdge;
        SamplerAddressMode address_mode_w = SamplerAddressMode::ClampToEdge;
        float mip_lod_bias = 0.0;
        uint16_t max_anisotropy = 1;
        CompareOperation compare_operation = CompareOperation::Less;
        float min_lod = 0.0;
        float max_lod = 1.0;
        BorderColor border_color = BorderColor::TransparentBlack;

        ResourceHandle handle;
    };

    class Sampler : public Resource
    {
    public:
        virtual ~Sampler() = default;

        [[nodiscard]] HE_FORCE_INLINE Filter mag_filter() const
        {
            return m_mag_filter;
        }

        [[nodiscard]] HE_FORCE_INLINE Filter min_filter() const
        {
            return m_min_filter;
        }

        [[nodiscard]] HE_FORCE_INLINE Filter mipmap_filter() const
        {
            return m_mipmap_filter;
        }

        [[nodiscard]] HE_FORCE_INLINE SamplerAddressMode address_mode_u() const
        {
            return m_address_mode_u;
        }

        [[nodiscard]] HE_FORCE_INLINE SamplerAddressMode address_mode_v() const
        {
            return m_address_mode_v;
        }

        [[nodiscard]] HE_FORCE_INLINE SamplerAddressMode address_mode_w() const
        {
            return m_address_mode_w;
        }

        [[nodiscard]] HE_FORCE_INLINE float mip_lod_bias() const
        {
            return m_mip_lod_bias;
        }

        [[nodiscard]] HE_FORCE_INLINE uint16_t max_anisotropy() const
        {
            return m_max_anisotropy;
        }

        [[nodiscard]] HE_FORCE_INLINE CompareOperation compare_operation() const
        {
            return m_compare_operation;
        }

        [[nodiscard]] HE_FORCE_INLINE float min_lod() const
        {
            return m_min_lod;
        }

        [[nodiscard]] HE_FORCE_INLINE float max_lod() const
        {
            return m_max_lod;
        }

        [[nodiscard]] HE_FORCE_INLINE BorderColor border_color() const
        {
            return m_border_color;
        }

        [[nodiscard]] HE_FORCE_INLINE ResourceHandle handle() const
        {
            return m_handle;
        }

    protected:
        explicit Sampler(const SamplerDescriptor &descriptor)
            : Resource(descriptor.label)
            , m_mag_filter(descriptor.mag_filter)
            , m_min_filter(descriptor.min_filter)
            , m_mipmap_filter(descriptor.mipmap_filter)
            , m_address_mode_u(descriptor.address_mode_u)
            , m_address_mode_v(descriptor.address_mode_v)
            , m_address_mode_w(descriptor.address_mode_w)
            , m_mip_lod_bias(descriptor.mip_lod_bias)
            , m_max_anisotropy(descriptor.max_anisotropy)
            , m_compare_operation(descriptor.compare_operation)
            , m_min_lod(descriptor.min_lod)
            , m_max_lod(descriptor.max_lod)
            , m_border_color(descriptor.border_color)
            , m_handle(descriptor.handle)
        {
            // TODO: Add assertions
        }

    protected:
        Filter m_mag_filter;
        Filter m_min_filter;
        Filter m_mipmap_filter;
        SamplerAddressMode m_address_mode_u;
        SamplerAddressMode m_address_mode_v;
        SamplerAddressMode m_address_mode_w;
        float m_mip_lod_bias;
        uint16_t m_max_anisotropy;
        CompareOperation m_compare_operation;
        float m_min_lod;
        float m_max_lod;
        BorderColor m_border_color;

        ResourceHandle m_handle;
    };
} // namespace hyper_rhi