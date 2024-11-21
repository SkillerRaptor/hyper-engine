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

namespace he
{
    namespace rhi
    {
        class IBuffer;
    } // namespace rhi

    namespace render
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
                std::shared_ptr<he::rhi::IBuffer> positions_buffer,
                std::shared_ptr<he::rhi::IBuffer> normals_buffer,
                std::shared_ptr<he::rhi::IBuffer> colors_buffer,
                std::shared_ptr<he::rhi::IBuffer> tex_coords_buffer,
                std::shared_ptr<he::rhi::IBuffer> mesh_buffer,
                std::shared_ptr<he::rhi::IBuffer> indices_buffer);

            std::string_view name() const;

            const std::vector<Surface> &surfaces() const;
            const std::shared_ptr<he::rhi::IBuffer> &positions_buffer() const;
            const std::shared_ptr<he::rhi::IBuffer> &normals_buffer() const;
            const std::shared_ptr<he::rhi::IBuffer> &colors_buffer() const;
            const std::shared_ptr<he::rhi::IBuffer> &tex_coords_buffer() const;
            const std::shared_ptr<he::rhi::IBuffer> &mesh_buffer() const;
            const std::shared_ptr<he::rhi::IBuffer> &indices_buffer() const;

        private:
            std::string m_name;
            std::vector<Surface> m_surfaces;

            std::shared_ptr<he::rhi::IBuffer> m_positions_buffer;
            std::shared_ptr<he::rhi::IBuffer> m_normals_buffer;
            std::shared_ptr<he::rhi::IBuffer> m_colors_buffer;
            std::shared_ptr<he::rhi::IBuffer> m_tex_coords_buffer;

            std::shared_ptr<he::rhi::IBuffer> m_mesh_buffer;
            std::shared_ptr<he::rhi::IBuffer> m_indices_buffer;
        };
    } // namespace render
} // namespace he