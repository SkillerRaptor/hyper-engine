/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <vector>

#include <hyper_core/math.hpp>

#include "hyper_render/material.hpp"
#include "hyper_render/mesh.hpp"

namespace hyper_render
{
    struct RenderObject
    {
        uint32_t index_count = 0;
        uint32_t first_index = 0;
        std::shared_ptr<hyper_rhi::Buffer> index_buffer = nullptr;

        MaterialInstance *material;

        glm::mat4 transform;
        std::shared_ptr<hyper_rhi::Buffer> mesh_buffer = nullptr;
    };

    struct DrawContext
    {
        std::vector<RenderObject> opaque_surfaces;
    };

    class Renderable
    {
    public:
        virtual ~Renderable() = default;

        virtual void draw(const glm::mat4 &top_matrix, DrawContext &context) const = 0;
    };

    class Node : public Renderable
    {
    public:
        Node(const glm::mat4 &world_transform, const glm::mat4 &local_transform)
            : m_parent()
            , m_children({})
            , m_world_transform(world_transform)
            , m_local_transform(local_transform)
        {
        }

        void refresh_transform(const glm::mat4 &parent_matrix);

        void draw(const glm::mat4 &top_matrix, DrawContext &context) const override;

    protected:
        std::weak_ptr<Node> m_parent;
        std::vector<std::shared_ptr<Node>> m_children;

        glm::mat4 m_world_transform;
        glm::mat4 m_local_transform;
    };

    class MeshNode final : public Node
    {
    public:
        MeshNode(const glm::mat4 &world_transform, const glm::mat4 &local_transform, const std::shared_ptr<Mesh> &mesh)
            : Node(world_transform, local_transform)
            , m_mesh(mesh)
        {
        }

        void draw(const glm::mat4 &top_matrix, DrawContext &context) const override;

        [[nodiscard]] HE_FORCE_INLINE Mesh &mesh() const
        {
            return *m_mesh;
        }

    private:
        std::shared_ptr<Mesh> m_mesh;
    };
} // namespace hyper_render