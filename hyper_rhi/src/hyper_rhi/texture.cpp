/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/texture.hpp"

#include <utility>

namespace hyper_rhi
{
    Texture::Texture(std::string label)
        : Resource(std::move(label))
        , m_handle(ResourceHandle(0xffffffff))
    {
    }

    ResourceHandle Texture::handle() const
    {
        return m_handle;
    }
} // namespace hyper_rhi