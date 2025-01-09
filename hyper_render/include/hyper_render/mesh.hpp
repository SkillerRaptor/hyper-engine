/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <vector>

#include <hyper_core/ref_counted.hpp>
#include <hyper_core/nonnull_ref_ptr.hpp>
#include <hyper_rhi/forward.hpp>

#include "hyper_render/material.hpp"

namespace hyper_engine
{
    struct GltfMaterial : RefCounted<GltfMaterial>
    {
        MaterialInstance data;
    };

    struct GltfSurface
    {
        uint32_t start_index = 0;
        uint32_t count = 0;
        RefPtr<GltfMaterial> material;
    };

    class Mesh : public RefCounted<Mesh>
    {
    public:
        Mesh(
            std::string name,
            std::vector<GltfSurface> surfaces,
            NonnullRefPtr<Buffer> positions_buffer,
            NonnullRefPtr<Buffer> normals_buffer,
            NonnullRefPtr<Buffer> colors_buffer,
            NonnullRefPtr<Buffer> tex_coords_buffer,
            NonnullRefPtr<Buffer> mesh_buffer,
            NonnullRefPtr<Buffer> indices_buffer);

        std::string_view name() const;

        const std::vector<GltfSurface> &surfaces() const;
        NonnullRefPtr<Buffer> positions_buffer() const;
        NonnullRefPtr<Buffer> normals_buffer() const;
        NonnullRefPtr<Buffer> colors_buffer() const;
        NonnullRefPtr<Buffer> tex_coords_buffer() const;
        NonnullRefPtr<Buffer> mesh_buffer() const;
        NonnullRefPtr<Buffer> indices_buffer() const;

    private:
        std::string m_name;
        std::vector<GltfSurface> m_surfaces;

        NonnullRefPtr<Buffer> m_positions_buffer;
        NonnullRefPtr<Buffer> m_normals_buffer;
        NonnullRefPtr<Buffer> m_colors_buffer;
        NonnullRefPtr<Buffer> m_tex_coords_buffer;

        NonnullRefPtr<Buffer> m_mesh_buffer;
        NonnullRefPtr<Buffer> m_indices_buffer;
    };
} // namespace hyper_engine