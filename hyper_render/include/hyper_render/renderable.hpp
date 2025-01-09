/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <vector>

#include <hyper_core/math.hpp>
#include <hyper_core/nonnull_ref_ptr.hpp>
#include <hyper_core/weak_ptr.hpp>
#include <hyper_rhi/forward.hpp>

#include "hyper_render/forward.hpp"
#include "hyper_render/material.hpp"
#include "hyper_render/mesh.hpp"

namespace hyper_engine
{
    struct RenderObject
    {
        uint32_t index_count = 0;
        uint32_t first_index = 0;
        NonnullRefPtr<Buffer> index_buffer;

        MaterialInstance *material = nullptr;

        glm::mat4 transform;
        NonnullRefPtr<Buffer> mesh_buffer;
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

    class Node
        : public Renderable
        , public Weakable<Node>
        , public RefCounted<Node>
    {
    public:
        void refresh_transform(const glm::mat4 &parent_matrix);

        void draw(const glm::mat4 &top_matrix, DrawContext &context) const override;

    public:
        WeakPtr<Node> parent;
        std::vector<RefPtr<Node>> children;

        glm::mat4 world_transform;
        glm::mat4 local_transform;
    };

    class MeshNode final : public Node
    {
    public:
        explicit MeshNode(const NonnullRefPtr<Mesh> &mesh);

        void draw(const glm::mat4 &top_matrix, DrawContext &context) const override;

    public:
        NonnullRefPtr<Mesh> mesh;
    };

    class LoadedGltf final
        : public Renderable
        , public RefCounted<LoadedGltf>
    {
    public:
        LoadedGltf(
            std::vector<NonnullRefPtr<Mesh>> meshes,
            std::vector<RefPtr<Node>> nodes,
            std::vector<NonnullRefPtr<Texture>> textures,
            std::vector<NonnullRefPtr<TextureView>> texture_views,
            std::vector<RefPtr<GltfMaterial>> materials,
            std::vector<RefPtr<Node>> top_nodes,
            std::vector<NonnullRefPtr<Sampler>> samplers);

        void draw(const glm::mat4 &top_matrix, DrawContext &draw_context) const override;

    private:
        std::vector<NonnullRefPtr<Mesh>> m_meshes;
        std::vector<RefPtr<Node>> m_nodes;
        std::vector<NonnullRefPtr<Texture>> m_textures;
        std::vector<NonnullRefPtr<TextureView>> m_texture_views;
        std::vector<RefPtr<GltfMaterial>> m_materials;
        std::vector<RefPtr<Node>> m_top_nodes;
        std::vector<NonnullRefPtr<Sampler>> m_samplers;
    };

    NonnullRefPtr<LoadedGltf> load_gltf(
        const NonnullRefPtr<CommandList> &command_list,
        const NonnullRefPtr<TextureView> &white_texture_view,
        const NonnullRefPtr<Texture> &error_texture,
        const NonnullRefPtr<TextureView> &error_texture_view,
        const NonnullRefPtr<Sampler> &default_sampler_linear,
        const GltfMetallicRoughness &metallic_roughness_material,
        const std::string &path);
} // namespace hyper_engine