/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include <hyper_core/math.hpp>

#include "hyper_render/material.hpp"

namespace hyper_engine
{
    struct GltfMaterial;
    class IBuffer;
    class ICommandList;
    class IGraphicsDevice;
    class ISampler;
    class ITexture;
    class ITextureView;
    class Mesh;

    struct RenderObject
    {
        uint32_t index_count = 0;
        uint32_t first_index = 0;
        std::shared_ptr<IBuffer> index_buffer = nullptr;

        MaterialInstance *material;

        glm::mat4 transform;
        std::shared_ptr<IBuffer> mesh_buffer = nullptr;
    };

    struct DrawContext
    {
        std::vector<RenderObject> opaque_surfaces;
        std::vector<RenderObject> transparent_surfaces;
    };

    class IRenderable
    {
    public:
        virtual ~IRenderable() = default;

        virtual void draw(const glm::mat4 &top_matrix, DrawContext &draw_context) const = 0;
    };

    class Node : public IRenderable
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
        explicit MeshNode(const std::shared_ptr<Mesh> &mesh);

        void draw(const glm::mat4 &top_matrix, DrawContext &context) const override;

    public:
        std::shared_ptr<Mesh> mesh;
    };

    class LoadedGltf final : public IRenderable
    {
    public:
        LoadedGltf(
            std::vector<std::shared_ptr<Mesh>> meshes,
            std::vector<std::shared_ptr<Node>> nodes,
            std::vector<std::shared_ptr<ITexture>> textures,
            std::vector<std::shared_ptr<ITextureView>> texture_views,
            std::vector<std::shared_ptr<GltfMaterial>> materials,
            std::vector<std::shared_ptr<Node>> top_nodes,
            std::vector<std::shared_ptr<ISampler>> samplers);

        void draw(const glm::mat4 &top_matrix, DrawContext &draw_context) const override;

    private:
        std::vector<std::shared_ptr<Mesh>> m_meshes;
        std::vector<std::shared_ptr<Node>> m_nodes;
        std::vector<std::shared_ptr<ITexture>> m_textures;
        std::vector<std::shared_ptr<ITextureView>> m_texture_views;
        std::vector<std::shared_ptr<GltfMaterial>> m_materials;
        std::vector<std::shared_ptr<Node>> m_top_nodes;
        std::vector<std::shared_ptr<ISampler>> m_samplers;
    };

    std::shared_ptr<LoadedGltf> load_gltf(
        const std::shared_ptr<IGraphicsDevice> &graphics_device,
        const std::shared_ptr<ICommandList> &command_list,
        const std::shared_ptr<ITextureView> &white_texture_view,
        const std::shared_ptr<ITexture> &error_texture,
        const std::shared_ptr<ITextureView> &error_texture_view,
        const std::shared_ptr<ISampler> &default_sampler_linear,
        const GltfMetallicRoughness &metallic_roughness_material,
        const std::string &path);
} // namespace hyper_engine