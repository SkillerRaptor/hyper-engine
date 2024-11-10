/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

#include <hyper_core/assertion.hpp>
#include <hyper_core/prerequisites.hpp>
#include <hyper_platform/window.hpp>

#include "hyper_rhi/texture.hpp"

namespace hyper_rhi
{
    class Surface
    {
    public:
        virtual ~Surface() = default;

        [[nodiscard]] virtual uint32_t min_image_count() const = 0;
        [[nodiscard]] virtual uint32_t image_count() const = 0;

        [[nodiscard]] virtual Format format() const = 0;

        [[nodiscard]] virtual std::shared_ptr<Texture> current_texture() const = 0;
        [[nodiscard]] virtual std::shared_ptr<TextureView> current_texture_view() const = 0;

        HE_FORCE_INLINE void resize(const uint32_t width, const uint32_t height)
        {
            if (m_width == width || m_height == height)
            {
                return;
            }

            m_resized = true;
            m_width = width;
            m_height = height;
        }

        [[nodiscard]] HE_FORCE_INLINE bool resized() const
        {
            return m_resized;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t width() const
        {
            return m_width;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t height() const
        {
            return m_height;
        }

    protected:
        Surface(const uint32_t width, const uint32_t height)
            : m_resized(false)
            , m_width(width)
            , m_height(height)
        {
            HE_ASSERT(width > 0);
            HE_ASSERT(height > 0);
        }

    protected:
        bool m_resized;
        uint32_t m_width;
        uint32_t m_height;
    };
} // namespace hyper_rhi
