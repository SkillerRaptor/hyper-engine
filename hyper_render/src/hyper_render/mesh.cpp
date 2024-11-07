/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/mesh.hpp"

#include <fastgltf/core.hpp>
#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/tools.hpp>

#include <hyper_core/logger.hpp>

#include "shader_interop.h"

namespace hyper_render
{
    std::optional<std::vector<std::shared_ptr<Mesh>>>
        Mesh::load_model(const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device, const std::string &path)
    {
        HE_INFO("Loading model: {}", path);

        const std::filesystem::path file_path(path);

        fastgltf::Parser parser;

        auto data = fastgltf::GltfDataBuffer::FromPath(file_path);
        if (data.error() != fastgltf::Error::None)
        {
            return std::nullopt;
        }

        auto asset = parser.loadGltf(data.get(), file_path.parent_path(), fastgltf::Options::LoadExternalBuffers);
        if (asset.error() != fastgltf::Error::None)
        {
            return std::nullopt;
        }

        std::vector<std::shared_ptr<Mesh>> meshes;

        struct Vertex
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec4 color;
        };

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        for (const auto &[primitives, weights, name] : asset->meshes)
        {
            Mesh mesh_asset;
            mesh_asset.name = name;

            vertices.clear();
            indices.clear();

            for (const fastgltf::Primitive &primitive : primitives)
            {
                Surface surface = {};
                surface.start_index = static_cast<uint32_t>(indices.size());
                surface.count = static_cast<uint32_t>(asset->accessors[primitive.indicesAccessor.value()].count);

                const auto initial_vertex = static_cast<uint32_t>(vertices.size());

                {
                    fastgltf::Accessor &accessor = asset->accessors[primitive.indicesAccessor.value()];

                    fastgltf::iterateAccessor<uint32_t>(
                        asset.get(),
                        accessor,
                        [&](const uint32_t index)
                        {
                            indices.push_back(index + initial_vertex);
                        });
                }

                {
                    fastgltf::Accessor &accessor = asset->accessors[primitive.findAttribute("POSITION")->accessorIndex];
                    vertices.resize(vertices.size() + accessor.count);

                    fastgltf::iterateAccessorWithIndex<glm::vec3>(
                        asset.get(),
                        accessor,
                        [&](const glm::vec3 value, const size_t index)
                        {
                            const Vertex vertex = {
                                .position = value,
                                .normal = glm::vec3(1.0, 1.0, 1.0),
                                .color = glm::vec4(1.0, 1.0, 1.0, 1.0),
                            };

                            vertices[initial_vertex + index] = vertex;
                        });
                }

                const fastgltf::Attribute *normals = primitive.findAttribute("NORMAL");
                if (normals != primitive.attributes.end())
                {
                    fastgltf::iterateAccessorWithIndex<glm::vec3>(
                        asset.get(),
                        asset->accessors[normals->accessorIndex],
                        [&](const glm::vec3 value, const size_t index)
                        {
                            vertices[initial_vertex + index].normal = value;
                        });
                }

                const fastgltf::Attribute *colors = primitive.findAttribute("COLOR_0");
                if (colors != primitive.attributes.end())
                {
                    fastgltf::iterateAccessorWithIndex<glm::vec4>(
                        asset.get(),
                        asset->accessors[normals->accessorIndex],
                        [&](const glm::vec4 value, const size_t index)
                        {
                            vertices[initial_vertex + index].color = value;
                        });
                }

                mesh_asset.surfaces.push_back(surface);
            }

            float scale_size = 0.01f;
            glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(scale_size, scale_size, scale_size));

            std::vector<glm::vec4> positions;
            std::vector<glm::vec4> normals;
            std::vector<glm::vec4> colors;
            for (const auto &[position, normal, color] : vertices)
            {
                positions.emplace_back(-position.x, position.z, position.y, 1.0);
                normals.emplace_back(-normal.x, normal.z, normal.y, 1.0);
                colors.emplace_back(color);

                positions[positions.size() - 1] = scale * positions[positions.size() - 1];
            }

            mesh_asset.positions_buffer = graphics_device->create_buffer({
                .label = fmt::format("{} Positions", mesh_asset.name),
                .byte_size = positions.size() * sizeof(glm::vec4),
                .usage = hyper_rhi::BufferUsage::Storage | hyper_rhi::BufferUsage::ShaderResource,
            });
            mesh_asset.positions = positions;

            mesh_asset.normals_buffer = graphics_device->create_buffer({
                .label = fmt::format("{} Normals", mesh_asset.name),
                .byte_size = normals.size() * sizeof(glm::vec4),
                .usage = hyper_rhi::BufferUsage::Storage | hyper_rhi::BufferUsage::ShaderResource,
            });
            mesh_asset.normals = normals;

            mesh_asset.colors_buffer = graphics_device->create_buffer({
                .label = fmt::format("{} Colors", mesh_asset.name),
                .byte_size = colors.size() * sizeof(glm::vec4),
                .usage = hyper_rhi::BufferUsage::Storage | hyper_rhi::BufferUsage::ShaderResource,
            });
            mesh_asset.colors = colors;

            mesh_asset.mesh_buffer = graphics_device->create_buffer({
                .label = fmt::format("{} Mesh Data", mesh_asset.name),
                .byte_size = sizeof(ShaderMesh),
                .usage = hyper_rhi::BufferUsage::Storage | hyper_rhi::BufferUsage::ShaderResource,
            });

            for (size_t index = 0; index < indices.size(); index += 3)
            {
                std::swap(indices[index], indices[index + 2]);
            }

            mesh_asset.indices_buffer = graphics_device->create_buffer({
                .label = fmt::format("{} Indices", mesh_asset.name),
                .byte_size = indices.size() * sizeof(uint32_t),
                .usage = hyper_rhi::BufferUsage::Index,
            });
            mesh_asset.indices = indices;

            meshes.push_back(std::make_shared<Mesh>(std::move(mesh_asset)));
        }

        // queue->submit(nullptr);

        return meshes;
    }
} // namespace hyper_render