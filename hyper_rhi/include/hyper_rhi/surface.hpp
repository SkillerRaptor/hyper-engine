/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/ref_counted.hpp>
#include <hyper_core/nonnull_ref_ptr.hpp>

#include "hyper_rhi/format.hpp"
#include "hyper_rhi/forward.hpp"

namespace hyper_engine
{
    class Surface : public RefCounted<Surface>
    {
    public:
        virtual ~Surface() = default;

        void resize(uint32_t width, uint32_t height);

        bool resized() const;
        uint32_t width() const;
        uint32_t height() const;

        virtual uint32_t min_image_count() const = 0;
        virtual uint32_t image_count() const = 0;

        virtual Format format() const = 0;

        virtual NonnullRefPtr<Texture> current_texture() const = 0;
        virtual NonnullRefPtr<TextureView> current_texture_view() const = 0;

    protected:
        bool m_resized = false;
        uint32_t m_width = 1;
        uint32_t m_height = 1;
    };
} // namespace hyper_engine
