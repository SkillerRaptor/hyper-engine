/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <vector>

#include <hyper_core/math.hpp>
#include <hyper_rhi/buffer.hpp>
#include <hyper_rhi/render_pass.hpp>

namespace hyper_render
{
    class Mesh
    {
    public:
        Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> tex_coords);

        void draw(const std::shared_ptr<hyper_rhi::RenderPass> &render_pass) const;

    private:
        std::vector<glm::vec3> m_positions;
        std::vector<glm::vec3> m_normals;
        std::vector<glm::vec2> m_tex_coords;
    };
} // namespace hyper_render