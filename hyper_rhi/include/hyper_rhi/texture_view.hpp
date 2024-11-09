/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/assertion.hpp>
#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/resource.hpp"
#include "hyper_rhi/resource_handle.hpp"
#include "hyper_rhi/texture.hpp"

namespace hyper_rhi
{
    enum class TextureComponentSwizzle
    {
        Identity,
        Zero,
        One,
        R,
        G,
        B,
        A,
    };

    struct TextureComponentMapping
    {
        TextureComponentSwizzle r = TextureComponentSwizzle::Identity;
        TextureComponentSwizzle g = TextureComponentSwizzle::Identity;
        TextureComponentSwizzle b = TextureComponentSwizzle::Identity;
        TextureComponentSwizzle a = TextureComponentSwizzle::Identity;
    };

    struct TextureViewDescriptor
    {
        std::string label;

        std::shared_ptr<Texture> texture = nullptr;

        uint32_t base_mip_level = 0;
        uint32_t mip_level_count = 1;
        uint32_t base_array_level = 0;
        uint32_t array_layer_count = 1;
        TextureComponentMapping component_mapping = {};
        Format format = Format::Unknown;
        TextureDimension dimension = TextureDimension::Unknown;

        ResourceHandle handle = {};
    };

    class TextureView : public Resource
    {
    public:
        virtual ~TextureView() = default;

        [[nodiscard]] HE_FORCE_INLINE Texture &texture() const
        {
            return *m_texture;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t base_mip_level() const
        {
            return m_base_mip_level;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t mip_level_count() const
        {
            return m_mip_level_count;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t base_array_level() const
        {
            return m_base_array_level;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t array_layer_count() const
        {
            return m_array_layer_count;
        }

        [[nodiscard]] HE_FORCE_INLINE TextureComponentMapping component_mapping() const
        {
            return m_component_mapping;
        }

        [[nodiscard]] HE_FORCE_INLINE Format format() const
        {
            return m_format;
        }

        [[nodiscard]] HE_FORCE_INLINE TextureDimension dimension() const
        {
            return m_dimension;
        }

        [[nodiscard]] HE_FORCE_INLINE ResourceHandle handle() const
        {
            return m_handle;
        }

    protected:
        explicit TextureView(const TextureViewDescriptor &descriptor)
            : Resource(descriptor.label)
            , m_texture(descriptor.texture)
            , m_base_mip_level(descriptor.base_mip_level)
            , m_mip_level_count(descriptor.mip_level_count)
            , m_base_array_level(descriptor.base_array_level)
            , m_array_layer_count(descriptor.array_layer_count)
            , m_component_mapping(descriptor.component_mapping)
            , m_format(descriptor.format)
            , m_dimension(descriptor.dimension)
            , m_handle(descriptor.handle)
        {
            HE_ASSERT(m_texture != nullptr);
            HE_ASSERT(m_mip_level_count > 0);
            HE_ASSERT(m_array_layer_count > 0);
            HE_ASSERT(m_format != Format::Unknown);
            HE_ASSERT(m_dimension != TextureDimension::Unknown);
        }

    protected:
        std::shared_ptr<Texture> m_texture;
        uint32_t m_base_mip_level;
        uint32_t m_mip_level_count;
        uint32_t m_base_array_level;
        uint32_t m_array_layer_count;
        TextureComponentMapping m_component_mapping;
        Format m_format;
        TextureDimension m_dimension;

        ResourceHandle m_handle;
    };
} // namespace hyper_rhi