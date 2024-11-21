/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/sampler.hpp"

#include <hyper_core/assertion.hpp>

namespace hyper_engine
{
    ISampler::ISampler(const SamplerDescriptor &descriptor)
        : m_label(descriptor.label)
        , m_mag_filter(descriptor.mag_filter)
        , m_min_filter(descriptor.min_filter)
        , m_mipmap_filter(descriptor.mipmap_filter)
        , m_address_mode_u(descriptor.address_mode_u)
        , m_address_mode_v(descriptor.address_mode_v)
        , m_address_mode_w(descriptor.address_mode_w)
        , m_mip_lod_bias(descriptor.mip_lod_bias)
        , m_compare_operation(descriptor.compare_operation)
        , m_min_lod(descriptor.min_lod)
        , m_max_lod(descriptor.max_lod)
        , m_border_color(descriptor.border_color)
        , m_handle(descriptor.handle)
    {
        HE_ASSERT(!m_label.empty(), "Sampler wasn't given a label");

        // TODO: Add assertions
    }

    std::string_view ISampler::label() const
    {
        return m_label;
    }

    Filter ISampler::mag_filter() const
    {
        return m_mag_filter;
    }

    Filter ISampler::min_filter() const
    {
        return m_min_filter;
    }

    Filter ISampler::mipmap_filter() const
    {
        return m_mipmap_filter;
    }

    AddressMode ISampler::address_mode_u() const
    {
        return m_address_mode_u;
    }

    AddressMode ISampler::address_mode_v() const
    {
        return m_address_mode_v;
    }

    AddressMode ISampler::address_mode_w() const
    {
        return m_address_mode_w;
    }

    float ISampler::mip_lod_bias() const
    {
        return m_mip_lod_bias;
    }

    CompareOperation ISampler::compare_operation() const
    {
        return m_compare_operation;
    }

    float ISampler::min_lod() const
    {
        return m_min_lod;
    }

    float ISampler::max_lod() const
    {
        return m_max_lod;
    }

    BorderColor ISampler::border_color() const
    {
        return m_border_color;
    }

    ResourceHandle ISampler::handle() const
    {
        return m_handle;
    }
} // namespace hyper_engine