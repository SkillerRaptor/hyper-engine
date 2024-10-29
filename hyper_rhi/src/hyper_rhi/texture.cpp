/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/texture.hpp"

namespace hyper_rhi
{
    Texture::Texture(const TextureDescriptor &descriptor)
        : Resource(descriptor.label)
        , m_width(descriptor.width)
        , m_height(descriptor.height)
        , m_handle(ResourceHandle(0xffffffff))
    {
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