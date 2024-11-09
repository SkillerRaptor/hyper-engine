/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <utility>
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
        std::vector<RenderObject> transparent_surfaces;
    };

    class Renderable
    {
    public:
        virtual ~Renderable() = default;

        virtual void draw(const glm::mat4 &top_matrix, DrawContext &draw_context) const = 0;
    };

    class Node : public Renderable
    {
    public:
        void refresh_transform(const glm::mat4 &parent_matrix);

        void draw(const glm::mat4 &top_matrix, DrawContext &context) const override;

    public:
        std::weak_ptr<Node> parent;
        std::vector<std::shared_ptr<Node>> children;

        glm::mat4 world_transform{};
        glm::mat4 local_transform{};
    };

    class MeshNode final : public Node
    {
    public:
        explicit MeshNode(const std::shared_ptr<Mesh> &mesh)
            : mesh(mesh)
        {
        }

        void draw(const glm::mat4 &top_matrix, DrawContext &context) const override;

    public:
        std::shared_ptr<Mesh> mesh;
    };

    class LoadedGltf final : public Renderable
    {
    public:
        LoadedGltf(
            std::vector<std::shared_ptr<Mesh>> meshes,
            std::vector<std::shared_ptr<Node>> nodes,
            std::vector<std::shared_ptr<hyper_rhi::Texture>> textures,
            std::vector<std::shared_ptr<hyper_rhi::TextureView>> texture_views,
            std::vector<std::shared_ptr<GltfMaterial>> materials,
            std::vector<std::shared_ptr<Node>> top_nodes,
            std::vector<std::shared_ptr<hyper_rhi::Sampler>> samplers)
            : m_meshes(std::move(meshes))
            , m_nodes(std::move(nodes))
            , m_textures(std::move(textures))
            , m_texture_views(std::move(texture_views))
            , m_materials(std::move(materials))
            , m_top_nodes(std::move(top_nodes))
            , m_samplers(std::move(samplers))
        {
        }

        void draw(const glm::mat4 &top_matrix, DrawContext &draw_context) const override;

    private:
        std::vector<std::shared_ptr<Mesh>> m_meshes;
        std::vector<std::shared_ptr<Node>> m_nodes;
        std::vector<std::shared_ptr<hyper_rhi::Texture>> m_textures;
        std::vector<std::shared_ptr<hyper_rhi::TextureView>> m_texture_views;
        std::vector<std::shared_ptr<GltfMaterial>> m_materials;
        std::vector<std::shared_ptr<Node>> m_top_nodes;
        std::vector<std::shared_ptr<hyper_rhi::Sampler>> m_samplers;
    };

    std::shared_ptr<LoadedGltf> load_gltf(
        const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device,
        const std::shared_ptr<hyper_rhi::CommandList> &command_list,
        const std::shared_ptr<hyper_rhi::TextureView> &white_texture_view,
        const std::shared_ptr<hyper_rhi::Texture> &error_texture,
        const std::shared_ptr<hyper_rhi::TextureView> &error_texture_view,
        const std::shared_ptr<hyper_rhi::Sampler> &default_sampler_linear,
        const GltfMetallicRoughness &metallic_roughness_material,
        const std::string &path);
} // namespace hyper_render