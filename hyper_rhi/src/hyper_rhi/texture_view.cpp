/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/texture_view.hpp"

#include <hyper_core/assertion.hpp>

namespace hyper_rhi
{
    TextureView::TextureView(const TextureViewDescriptor &descriptor)
        : Resource(descriptor.label)
        , m_base_mip_level(descriptor.base_mip_level)
        , m_mip_level_count(descriptor.mip_level_count)
        , m_base_array_level(descriptor.base_array_level)
        , m_array_layer_count(descriptor.array_layer_count)
        , m_component_mapping(descriptor.component_mapping)
        , m_format(descriptor.format)
        , m_dimension(descriptor.dimension)
    {
        HE_ASSERT(m_mip_level_count > 0);
        HE_ASSERT(m_array_layer_count > 0);
    }

    uint32_t TextureView::base_mip_level() const
    {
        return m_base_mip_level;
    }

    uint32_t TextureView::mip_level_count() const
    {
        return m_mip_level_count;
    }

    uint32_t TextureView::base_array_level() const
    {
        return m_base_array_level;
    }

    uint32_t TextureView::array_layer_count() const
    {
        return m_array_layer_count;
    }

    TextureComponentMapping TextureView::component_mapping() const
    {
        return m_component_mapping;
    }

    TextureFormat TextureView::format() const
    {
        return m_format;
    }

    TextureDimension TextureView::dimension() const
    {
        return m_dimension;
    }
} // namespace hyper_rhi