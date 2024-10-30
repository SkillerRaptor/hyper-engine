/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/texture.hpp"

#include <hyper_core/assertion.hpp>

namespace hyper_rhi
{
    Texture::Texture(const TextureDescriptor &descriptor)
        : Resource(descriptor.label)
        , m_width(descriptor.width)
        , m_height(descriptor.height)
        , m_depth(descriptor.depth)
        , m_array_size(descriptor.array_size)
        , m_mip_levels(descriptor.mip_levels)
        , m_format(descriptor.format)
        , m_dimension(descriptor.dimension)
        , m_usage(descriptor.usage)
        , m_view(nullptr)
    {
        HE_ASSERT(m_width > 0);
        HE_ASSERT(m_height > 0);
        HE_ASSERT(m_depth > 0);
        HE_ASSERT(m_array_size > 0);
        HE_ASSERT(m_mip_levels > 0);
        HE_ASSERT(m_format != TextureFormat::Unknown);
        HE_ASSERT(m_dimension != TextureDimension::Unknown);
        HE_ASSERT(m_usage != TextureUsage::None);
    }

    uint32_t Texture::width() const
    {
        return m_width;
    }

    uint32_t Texture::height() const
    {
        return m_height;
    }

    uint32_t Texture::depth() const
    {
        return m_depth;
    }

    uint32_t Texture::array_size() const
    {
        return m_array_size;
    }

    uint32_t Texture::mip_levels() const
    {
        return m_mip_levels;
    }

    TextureFormat Texture::format() const
    {
        return m_format;
    }

    TextureDimension Texture::dimension() const
    {
        return m_dimension;
    }

    TextureUsage Texture::usage() const
    {
        return m_usage;
    }

    std::shared_ptr<TextureView> Texture::view() const
    {
        return m_view;
    }
} // namespace hyper_rhi