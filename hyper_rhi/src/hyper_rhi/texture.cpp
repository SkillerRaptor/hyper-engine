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
        , m_handle(descriptor.handle)
    {
        HE_ASSERT(descriptor.width > 0);
        HE_ASSERT(descriptor.height > 0);
        HE_ASSERT(descriptor.depth > 0);
        HE_ASSERT(descriptor.array_size > 0);
        HE_ASSERT(descriptor.mip_levels > 0);
        HE_ASSERT(descriptor.sample_count > 0);
        HE_ASSERT(descriptor.format != TextureFormat::Unknown);
        HE_ASSERT(descriptor.dimension != TextureDimension::Unknown);
    }

    uint32_t Texture::width() const
    {
        return m_width;
    }

    uint32_t Texture::height() const
    {
        return m_height;
    }

    ResourceHandle Texture::handle() const
    {
        return m_handle;
    }
} // namespace hyper_rhi