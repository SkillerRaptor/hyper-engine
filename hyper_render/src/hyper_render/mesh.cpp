/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/mesh.hpp"

namespace he::render
{
    Mesh::Mesh(
        std::string name,
        std::vector<Surface> surfaces,
        std::shared_ptr<he::rhi::IBuffer> positions_buffer,
        std::shared_ptr<he::rhi::IBuffer> normals_buffer,
        std::shared_ptr<he::rhi::IBuffer> colors_buffer,
        std::shared_ptr<he::rhi::IBuffer> tex_coords_buffer,
        std::shared_ptr<he::rhi::IBuffer> mesh_buffer,
        std::shared_ptr<he::rhi::IBuffer> indices_buffer)
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

    const std::shared_ptr<he::rhi::IBuffer> &Mesh::positions_buffer() const
    {
        return m_positions_buffer;
    }

    const std::shared_ptr<he::rhi::IBuffer> &Mesh::normals_buffer() const
    {
        return m_normals_buffer;
    }

    const std::shared_ptr<he::rhi::IBuffer> &Mesh::colors_buffer() const
    {
        return m_colors_buffer;
    }

    const std::shared_ptr<he::rhi::IBuffer> &Mesh::tex_coords_buffer() const
    {
        return m_tex_coords_buffer;
    }

    const std::shared_ptr<he::rhi::IBuffer> &Mesh::mesh_buffer() const
    {
        return m_mesh_buffer;
    }

    const std::shared_ptr<he::rhi::IBuffer> &Mesh::indices_buffer() const
    {
        return m_indices_buffer;
    }
} // namespace he::render