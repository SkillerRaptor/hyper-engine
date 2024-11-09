/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <vector>

#include <hyper_core/prerequisites.hpp>
#include <hyper_rhi/graphics_device.hpp>

#include "hyper_render/material.hpp"

namespace hyper_render
{
    struct GltfMaterial
    {
        MaterialInstance data;
    };

    struct Surface
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
            std::vector<Surface> surfaces,
            std::shared_ptr<hyper_rhi::Buffer> positions_buffer,
            std::shared_ptr<hyper_rhi::Buffer> normals_buffer,
            std::shared_ptr<hyper_rhi::Buffer> colors_buffer,
            std::shared_ptr<hyper_rhi::Buffer> tex_coords_buffer,
            std::shared_ptr<hyper_rhi::Buffer> mesh_buffer,
            std::shared_ptr<hyper_rhi::Buffer> indices_buffer)
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

        [[nodiscard]] HE_FORCE_INLINE std::string_view name() const
        {
            return m_name;
        }

        [[nodiscard]] HE_FORCE_INLINE std::vector<Surface> &surfaces()
        {
            return m_surfaces;
        }

        [[nodiscard]] HE_FORCE_INLINE std::shared_ptr<hyper_rhi::Buffer> positions_buffer() const
        {
            return m_positions_buffer;
        }

        [[nodiscard]] HE_FORCE_INLINE std::shared_ptr<hyper_rhi::Buffer> normals_buffer() const
        {
            return m_normals_buffer;
        }

        [[nodiscard]] HE_FORCE_INLINE std::shared_ptr<hyper_rhi::Buffer> colors_buffer() const
        {
            return m_colors_buffer;
        }

        [[nodiscard]] HE_FORCE_INLINE std::shared_ptr<hyper_rhi::Buffer> tex_coords_buffer() const
        {
            return m_tex_coords_buffer;
        }

        [[nodiscard]] HE_FORCE_INLINE std::shared_ptr<hyper_rhi::Buffer> mesh_buffer() const
        {
            return m_mesh_buffer;
        }

        [[nodiscard]] HE_FORCE_INLINE std::shared_ptr<hyper_rhi::Buffer> indices_buffer() const
        {
            return m_indices_buffer;
        }

    private:
        std::string m_name;
        std::vector<Surface> m_surfaces;

        std::shared_ptr<hyper_rhi::Buffer> m_positions_buffer;
        std::shared_ptr<hyper_rhi::Buffer> m_normals_buffer;
        std::shared_ptr<hyper_rhi::Buffer> m_colors_buffer;
        std::shared_ptr<hyper_rhi::Buffer> m_tex_coords_buffer;

        std::shared_ptr<hyper_rhi::Buffer> m_mesh_buffer;
        std::shared_ptr<hyper_rhi::Buffer> m_indices_buffer;
    };
} // namespace hyper_render