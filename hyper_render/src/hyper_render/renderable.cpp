/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/renderable.hpp"

namespace hyper_render
{
    void Node::refresh_transform(const glm::mat4 &parent_matrix)
    {
        m_world_transform = parent_matrix * m_local_transform;

        for (const std::shared_ptr<Node> &child : m_children)
        {
            child->refresh_transform(m_world_transform);
        }
    }

    void Node::draw(const glm::mat4 &top_matrix, DrawContext &context) const
    {
        for (const std::shared_ptr<Node> &child : m_children)
        {
            child->draw(top_matrix, context);
        }
    }

    void MeshNode::draw(const glm::mat4 &top_matrix, DrawContext &context) const
    {
        const glm::mat4 node_matrix = top_matrix * m_world_transform;

        for (const Surface &surface : m_mesh->surfaces())
        {
            const RenderObject render_object = {
                .index_count = surface.count,
                .first_index = surface.start_index,
                .index_buffer = m_mesh->indices_buffer(),
                .material = &surface.material->data,
                .transform = node_matrix,
                .mesh_buffer = m_mesh->mesh_buffer(),
            };

            context.opaque_surfaces.push_back(render_object);
        }

        Node::draw(top_matrix, context);
    }
} // namespace hyper_render