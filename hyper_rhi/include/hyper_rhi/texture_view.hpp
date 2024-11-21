/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

#include "hyper_rhi/resource_handle.hpp"
#include "hyper_rhi/subresource_range.hpp"

namespace hyper_engine
{
    class ITexture;

    enum class ComponentSwizzle : uint8_t
    {
        Identity,
        Zero,
        One,
        R,
        G,
        B,
        A,
    };

    struct ComponentMapping
    {
        ComponentSwizzle r = ComponentSwizzle::Identity;
        ComponentSwizzle g = ComponentSwizzle::Identity;
        ComponentSwizzle b = ComponentSwizzle::Identity;
        ComponentSwizzle a = ComponentSwizzle::Identity;
    };

    struct TextureViewDescriptor
    {
        std::string label;

        std::shared_ptr<ITexture> texture = nullptr;

        SubresourceRange subresource_range;
        ComponentMapping component_mapping = {};

        ResourceHandle handle = {};
    };

    class ITextureView
    {
    public:
        virtual ~ITextureView() = default;

        std::string_view label() const;

        const std::shared_ptr<ITexture> &texture() const;
        SubresourceRange subresource_range() const;
        ComponentMapping component_mapping() const;

        ResourceHandle handle() const;

    protected:
        explicit ITextureView(const TextureViewDescriptor &descriptor);

    protected:
        std::string m_label;

        std::shared_ptr<ITexture> m_texture;
        SubresourceRange m_subresource_range;
        ComponentMapping m_component_mapping;

        ResourceHandle m_handle;
    };
} // namespace hyper_engine