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
#include <utility>

#include "shader_interop.h"

namespace hyper_render
{
    std::vector<std::shared_ptr<Mesh>> Mesh::load_model(
        const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device,
        const std::shared_ptr<hyper_rhi::CommandList> &command_list,
        const std::string &path)
    {
        HE_INFO("Loading model '{}'", path);

        const std::filesystem::path file_path(path);

        fastgltf::Expected<fastgltf::GltfDataBuffer> data = fastgltf::GltfDataBuffer::FromPath(file_path);
        HE_ASSERT(data.error() == fastgltf::Error::None);

        fastgltf::Parser parser;
        fastgltf::Expected<fastgltf::Asset> asset = parser.loadGltf(data.get(), file_path.parent_path(), fastgltf::Options::LoadExternalBuffers);
        HE_ASSERT(asset.error() == fastgltf::Error::None);

        std::vector<std::shared_ptr<Mesh>> meshes;

        std::vector<glm::vec4> positions;
        std::vector<glm::vec4> normals;
        std::vector<glm::vec4> colors;
        std::vector<glm::vec4> tex_coords;
        std::vector<uint32_t> indices;
        for (const fastgltf::Mesh &mesh : asset->meshes)
        {
            positions.clear();
            normals.clear();
            colors.clear();
            tex_coords.clear();
            indices.clear();

            std::vector<Surface> surfaces;
            for (const fastgltf::Primitive &primitive : mesh.primitives)
            {
                const Surface surface = {
                    .start_index = static_cast<uint32_t>(indices.size()),
                    .count = static_cast<uint32_t>(asset->accessors[primitive.indicesAccessor.value()].count),
                    .material = nullptr,
                };

                const auto initial_vertex = static_cast<uint32_t>(positions.size());

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
                    fastgltf::Accessor &positions_attribute = asset->accessors[primitive.findAttribute("POSITION")->accessorIndex];
                    positions.resize(positions.size() + positions_attribute.count);
                    normals.resize(normals.size() + positions_attribute.count);
                    colors.resize(colors.size() + positions_attribute.count);
                    tex_coords.resize(tex_coords.size() + positions_attribute.count);

                    fastgltf::iterateAccessorWithIndex<glm::vec3>(
                        asset.get(),
                        positions_attribute,
                        [&](const glm::vec3 value, const size_t index)
                        {
                            positions[initial_vertex + index] = glm::vec4(-value.x, value.z, value.y, 1.0);
                        });
                }

                const fastgltf::Attribute *colors_attribute = primitive.findAttribute("COLOR_0");
                if (colors_attribute != primitive.attributes.end())
                {
                    fastgltf::iterateAccessorWithIndex<glm::vec4>(
                        asset.get(),
                        asset->accessors[colors_attribute->accessorIndex],
                        [&](const glm::vec4 value, const size_t index)
                        {
                            colors[initial_vertex + index] = value;
                        });
                }

                const fastgltf::Attribute *normals_attribute = primitive.findAttribute("NORMAL");
                if (normals_attribute != primitive.attributes.end())
                {
                    fastgltf::iterateAccessorWithIndex<glm::vec3>(
                        asset.get(),
                        asset->accessors[normals_attribute->accessorIndex],
                        [&](const glm::vec3 value, const size_t index)
                        {
                            normals[initial_vertex + index] = glm::vec4(value.x, value.y, value.z, 1.0);
                            if (colors_attribute == primitive.attributes.end())
                            {
                                colors[initial_vertex + index] = normals[initial_vertex + index];
                            }
                        });
                }

                surfaces.push_back(surface);
            }

            for (size_t index = 0; index < indices.size(); index += 3)
            {
                std::swap(indices[index], indices[index + 2]);
            }

            const std::shared_ptr<hyper_rhi::Buffer> positions_buffer = graphics_device->create_buffer({
                .label = fmt::format("{} Positions", mesh.name),
                .byte_size = positions.size() * sizeof(glm::vec4),
                .usage = hyper_rhi::BufferUsage::Storage | hyper_rhi::BufferUsage::ShaderResource,
            });
            command_list->write_buffer(positions_buffer, positions.data(), positions.size() * sizeof(glm::vec4), 0);

            const std::shared_ptr<hyper_rhi::Buffer> normals_buffer = graphics_device->create_buffer({
                .label = fmt::format("{} Normals", mesh.name),
                .byte_size = normals.size() * sizeof(glm::vec4),
                .usage = hyper_rhi::BufferUsage::Storage | hyper_rhi::BufferUsage::ShaderResource,
            });
            command_list->write_buffer(normals_buffer, normals.data(), normals.size() * sizeof(glm::vec4), 0);

            const std::shared_ptr<hyper_rhi::Buffer> colors_buffer = graphics_device->create_buffer({
                .label = fmt::format("{} Colors", mesh.name),
                .byte_size = colors.size() * sizeof(glm::vec4),
                .usage = hyper_rhi::BufferUsage::Storage | hyper_rhi::BufferUsage::ShaderResource,
            });
            command_list->write_buffer(colors_buffer, colors.data(), colors.size() * sizeof(glm::vec4), 0);

            const std::shared_ptr<hyper_rhi::Buffer> tex_coords_buffer = graphics_device->create_buffer({
                .label = fmt::format("{} Tex Coords", mesh.name),
                .byte_size = tex_coords.size() * sizeof(glm::vec4),
                .usage = hyper_rhi::BufferUsage::Storage | hyper_rhi::BufferUsage::ShaderResource,
            });
            command_list->write_buffer(tex_coords_buffer, tex_coords.data(), tex_coords.size() * sizeof(glm::vec4), 0);

            const std::shared_ptr<hyper_rhi::Buffer> mesh_buffer = graphics_device->create_buffer({
                .label = fmt::format("{} Mesh Data", mesh.name),
                .byte_size = sizeof(ShaderMesh),
                .usage = hyper_rhi::BufferUsage::Storage | hyper_rhi::BufferUsage::ShaderResource,
            });

            const ShaderMesh shader_mesh = {
                .positions = positions_buffer->handle(),
                .normals = normals_buffer->handle(),
                .colors = colors_buffer->handle(),
                .tex_coords = tex_coords_buffer->handle(),
            };

            command_list->write_buffer(mesh_buffer, &shader_mesh, sizeof(ShaderMesh), 0);

            const std::shared_ptr<hyper_rhi::Buffer> indices_buffer = graphics_device->create_buffer({
                .label = fmt::format("{} Indices", mesh.name),
                .byte_size = indices.size() * sizeof(uint32_t),
                .usage = hyper_rhi::BufferUsage::Index,
            });
            command_list->write_buffer(indices_buffer, indices.data(), indices.size() * sizeof(uint32_t), 0);

            meshes.push_back(std::make_shared<Mesh>(
                std::string(mesh.name),
                surfaces,
                positions_buffer,
                normals_buffer,
                colors_buffer,
                tex_coords_buffer,
                mesh_buffer,
                indices_buffer));
        }

        return meshes;
    }
} // namespace hyper_render