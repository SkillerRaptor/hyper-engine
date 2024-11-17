/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/texture_view.hpp"

#include <hyper_core/assertion.hpp>

namespace he::rhi
{
    TextureView::TextureView(const TextureViewDescriptor &descriptor)
        : m_label(descriptor.label)
        , m_texture(descriptor.texture)
        , m_subresource_range(descriptor.subresource_range)
        , m_component_mapping(descriptor.component_mapping)
        , m_handle(descriptor.handle)
    {
        HE_ASSERT(!m_label.empty(), "Texture View wasn't given a label");

        HE_ASSERT(m_texture != nullptr);
        HE_ASSERT(m_subresource_range.mip_level_count > 0);
        HE_ASSERT(m_subresource_range.array_layer_count > 0);
    }

    std::string_view TextureView::label() const
    {
        return m_label;
    }

    const std::shared_ptr<Texture> &TextureView::texture() const
    {
        return m_texture;
    }

    SubresourceRange TextureView::subresource_range() const
    {
        return m_subresource_range;
    }

    ComponentMapping TextureView::component_mapping() const
    {
        return m_component_mapping;
    }

    ResourceHandle TextureView::handle() const
    {
        return m_handle;
    }
} // namespace he::rhi