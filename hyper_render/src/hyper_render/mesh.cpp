/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/mesh.hpp"

namespace hyper_engine
{
    Mesh::Mesh(
        std::string name,
        std::vector<Surface> surfaces,
        std::shared_ptr<IBuffer> positions_buffer,
        std::shared_ptr<IBuffer> normals_buffer,
        std::shared_ptr<IBuffer> colors_buffer,
        std::shared_ptr<IBuffer> tex_coords_buffer,
        std::shared_ptr<IBuffer> mesh_buffer,
        std::shared_ptr<IBuffer> indices_buffer)
        : m_name(std::move(name))
        , m_surfaces(std::move(surfaces))
        , m_positions_buffer(std::move(positions_buffer))
        , m_normals_buffer(std::move(normals_buffer))
        , m_colors_buffer(std::move(colors_buffer))
        , m_tex_coords_buffer(std::move(tex_coords_buffer))
        , m_mesh_buffer(std::move(mesh_buffer))
        , m_indices_buffer(std::move(indices_buffer))
    {
    }

    std::string_view Mesh::name() const
    {
        return m_name;
    }

    const std::vector<Surface> &Mesh::surfaces() const
    {
        return m_surfaces;
    }

    const std::shared_ptr<IBuffer> &Mesh::positions_buffer() const
    {
        return m_positions_buffer;
    }

    const std::shared_ptr<IBuffer> &Mesh::normals_buffer() const
    {
        return m_normals_buffer;
    }

    const std::shared_ptr<IBuffer> &Mesh::colors_buffer() const
    {
        return m_colors_buffer;
    }

    const std::shared_ptr<IBuffer> &Mesh::tex_coords_buffer() const
    {
        return m_tex_coords_buffer;
    }

    const std::shared_ptr<IBuffer> &Mesh::mesh_buffer() const
    {
        return m_mesh_buffer;
    }

    const std::shared_ptr<IBuffer> &Mesh::indices_buffer() const
    {
        return m_indices_buffer;
    }
} // namespace hyper_engine