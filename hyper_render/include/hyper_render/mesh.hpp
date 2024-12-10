/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "hyper_render/material.hpp"

namespace hyper_engine
{
    class Buffer;

    struct GltfMaterial
    {
        MaterialInstance data;
    };

    struct GltfSurface
    {
        uint32_t start_index;
        uint32_t count;
        std::shared_ptr<GltfMaterial> material;
    };

    class Mesh
    {
    public:
        Mesh(
            std::string name,
            std::vector<GltfSurface> surfaces,
            std::shared_ptr<Buffer> positions_buffer,
            std::shared_ptr<Buffer> normals_buffer,
            std::shared_ptr<Buffer> colors_buffer,
            std::shared_ptr<Buffer> tex_coords_buffer,
            std::shared_ptr<Buffer> mesh_buffer,
            std::shared_ptr<Buffer> indices_buffer);

        std::string_view name() const;

        const std::vector<GltfSurface> &surfaces() const;
        const std::shared_ptr<Buffer> &positions_buffer() const;
        const std::shared_ptr<Buffer> &normals_buffer() const;
        const std::shared_ptr<Buffer> &colors_buffer() const;
        const std::shared_ptr<Buffer> &tex_coords_buffer() const;
        const std::shared_ptr<Buffer> &mesh_buffer() const;
        const std::shared_ptr<Buffer> &indices_buffer() const;

    private:
        std::string m_name;
        std::vector<GltfSurface> m_surfaces;

        std::shared_ptr<Buffer> m_positions_buffer;
        std::shared_ptr<Buffer> m_normals_buffer;
        std::shared_ptr<Buffer> m_colors_buffer;
        std::shared_ptr<Buffer> m_tex_coords_buffer;

        std::shared_ptr<Buffer> m_mesh_buffer;
        std::shared_ptr<Buffer> m_indices_buffer;
    };
} // namespace hyper_engine