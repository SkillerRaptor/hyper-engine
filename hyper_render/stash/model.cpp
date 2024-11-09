/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/model.hpp"

#include <fastgltf/core.hpp>
#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/tools.hpp>
#include <stb_image.h>

#include <hyper_core/assertion.hpp>

namespace hyper_render
{
    Model::Model(const std::string &path)
    {
        const std::filesystem::path file_path(path);

        auto data = fastgltf::MappedGltfFile::FromPath(file_path);
        HE_ASSERT(data.error() == fastgltf::Error::None);

        constexpr fastgltf::Options options = fastgltf::Options::DontRequireValidAssetMember | fastgltf::Options::LoadExternalBuffers |
                                              fastgltf::Options::LoadExternalImages | fastgltf::Options::GenerateMeshIndices;

        fastgltf::Parser parser;
        auto asset = parser.loadGltf(data.get(), file_path.parent_path(), options);
        HE_ASSERT(asset.error() == fastgltf::Error::None);

        for (const fastgltf::Mesh &mesh : asset->meshes)
        {
            this->load_mesh(mesh);
        }

        for (const fastgltf::Image &image : asset->images)
        {
            this->load_image(image);
        }

        for (const fastgltf::Material &material : asset->materials)
        {
            this->load_material(material);
        }
    }

    void Model::draw(const std::shared_ptr<hyper_rhi::RenderPass> &render_pass) const
    {
        for (const Mesh &mesh : m_meshes)
        {
            mesh.draw(render_pass);
        }
    }

    void Model::load_mesh(const fastgltf::Mesh &mesh)
    {
        // Mesh outMesh = {};
        // outMesh.primitives.resize(mesh.primitives.size());

        for (const fastgltf::Primitive &primitive : mesh.primitives)
        {
            const fastgltf::Attribute *positions = primitive.findAttribute("POSITION");
            HE_ASSERT(positions != primitive.attributes.end());
            HE_ASSERT(primitive.indicesAccessor.has_value());

            /*
            // Generate the VAO
            GLuint vao = GL_NONE;
            glCreateVertexArrays(1, &vao);

            std::size_t baseColorTexcoordIndex = 0;

            // Get the output primitive
            auto index = std::distance(mesh.primitives.begin(), it);
            auto &primitive = outMesh.primitives[index];
            primitive.primitiveType = fastgltf::to_underlying(it->type);
            primitive.vertexArray = vao;
             */

            size_t base_color_tex_coord_index = 0;
            if (primitive.materialIndex.has_value())
            {
                primitive.materialUniformsIndex = it->materialIndex.value() + 1; // Adjust for default material
                auto &material = viewer->asset.materials[it->materialIndex.value()];

                auto &baseColorTexture = material.pbrData.baseColorTexture;
                if (baseColorTexture.has_value())
                {
                    auto &texture = viewer->asset.textures[baseColorTexture->textureIndex];
                    if (!texture.imageIndex.has_value())
                        return false;
                    primitive.albedoTexture = viewer->textures[texture.imageIndex.value()].texture;

                    if (baseColorTexture->transform && baseColorTexture->transform->texCoordIndex.has_value())
                    {
                        base_color_tex_coord_index = baseColorTexture->transform->texCoordIndex.value();
                    }
                    else
                    {
                        base_color_tex_coord_index = material.pbrData.baseColorTexture->texCoordIndex;
                    }
                }
            }
            else
            {
                primitive.materialUniformsIndex = 0;
            }

            /*

            {
                // Position
                auto &positionAccessor = asset.accessors[positionIt->accessorIndex];
                if (!positionAccessor.bufferViewIndex.has_value())
                    continue;

                // Create the vertex buffer for this primitive, and use the accessor tools to copy directly into the mapped buffer.
                glCreateBuffers(1, &primitive.vertexBuffer);
                glNamedBufferData(primitive.vertexBuffer, positionAccessor.count * sizeof(Vertex), nullptr, GL_STATIC_DRAW);
                auto *vertices = static_cast<Vertex *>(glMapNamedBuffer(primitive.vertexBuffer, GL_WRITE_ONLY));
                fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(
                    asset,
                    positionAccessor,
                    [&](fastgltf::math::fvec3 pos, std::size_t idx)
                    {
                        vertices[idx].position = fastgltf::math::fvec3(pos.x(), pos.y(), pos.z());
                        vertices[idx].uv = fastgltf::math::fvec2();
                    });
                glUnmapNamedBuffer(primitive.vertexBuffer);

                glEnableVertexArrayAttrib(vao, 0);
                glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
                glVertexArrayAttribBinding(vao, 0, 0);

                glVertexArrayVertexBuffer(vao, 0, primitive.vertexBuffer, 0, sizeof(Vertex));
            }

            auto texcoordAttribute = std::string("TEXCOORD_") + std::to_string(baseColorTexcoordIndex);
            if (const auto *texcoord = it->findAttribute(texcoordAttribute); texcoord != it->attributes.end())
            {
                // Tex coord
                auto &texCoordAccessor = asset.accessors[texcoord->accessorIndex];
                if (!texCoordAccessor.bufferViewIndex.has_value())
                    continue;

                auto *vertices = static_cast<Vertex *>(glMapNamedBuffer(primitive.vertexBuffer, GL_WRITE_ONLY));
                fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec2>(
                    asset,
                    texCoordAccessor,
                    [&](fastgltf::math::fvec2 uv, std::size_t idx)
                    {
                        vertices[idx].uv = fastgltf::math::fvec2(uv.x(), uv.y());
                    });
                glUnmapNamedBuffer(primitive.vertexBuffer);

                glEnableVertexArrayAttrib(vao, 1);
                glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
                glVertexArrayAttribBinding(vao, 1, 1);

                glVertexArrayVertexBuffer(vao, 1, primitive.vertexBuffer, offsetof(Vertex, uv), sizeof(Vertex));
            }

            // Generate the indirect draw command
            auto &draw = primitive.draw;
            draw.instanceCount = 1;
            draw.baseInstance = 0;
            draw.baseVertex = 0;
            draw.firstIndex = 0;

            auto &indexAccessor = asset.accessors[it->indicesAccessor.value()];
            if (!indexAccessor.bufferViewIndex.has_value())
                return false;
            draw.count = static_cast<std::uint32_t>(indexAccessor.count);

            // Create the index buffer and copy the indices into it.
            glCreateBuffers(1, &primitive.indexBuffer);
            if (indexAccessor.componentType == fastgltf::ComponentType::UnsignedByte ||
                indexAccessor.componentType == fastgltf::ComponentType::UnsignedShort)
            {
                primitive.indexType = GL_UNSIGNED_SHORT;
                glNamedBufferData(
                    primitive.indexBuffer, static_cast<GLsizeiptr>(indexAccessor.count * sizeof(std::uint16_t)), nullptr, GL_STATIC_DRAW);
                auto *indices = static_cast<std::uint16_t *>(glMapNamedBuffer(primitive.indexBuffer, GL_WRITE_ONLY));
                fastgltf::copyFromAccessor<std::uint16_t>(asset, indexAccessor, indices);
                glUnmapNamedBuffer(primitive.indexBuffer);
            }
            else
            {
                primitive.indexType = GL_UNSIGNED_INT;
                glNamedBufferData(
                    primitive.indexBuffer, static_cast<GLsizeiptr>(indexAccessor.count * sizeof(std::uint32_t)), nullptr, GL_STATIC_DRAW);
                auto *indices = static_cast<std::uint32_t *>(glMapNamedBuffer(primitive.indexBuffer, GL_WRITE_ONLY));
                fastgltf::copyFromAccessor<std::uint32_t>(asset, indexAccessor, indices);
                glUnmapNamedBuffer(primitive.indexBuffer);
            }

            glVertexArrayElementBuffer(vao, primitive.indexBuffer);
            */
        }

        /*
        // Create the buffer holding all of our primitive structs.
        glCreateBuffers(1, &outMesh.drawsBuffer);
        glNamedBufferData(
            outMesh.drawsBuffer,
            static_cast<GLsizeiptr>(outMesh.primitives.size() * sizeof(Primitive)),
            outMesh.primitives.data(),
            GL_STATIC_DRAW);

        viewer->meshes.emplace_back(outMesh);
        */
    }

    void Model::load_image(
        const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device,
        const std::shared_ptr<hyper_rhi::CommandList> &command_list,
        const fastgltf::Image &image)
    {
        /*
        auto getLevelCount = [](int width, int height) -> GLsizei
        {
            return static_cast<GLsizei>(1 + floor(log2(width > height ? width : height)));
        };
        */

        std::visit(
            fastgltf::visitor{
                [](auto &arg)
                {
                },
                [&](fastgltf::sources::URI &file_path)
                {
                    HE_ASSERT(file_path.fileByteOffset == 0);
                    HE_ASSERT(file_path.uri.isLocalPath());

                    int32_t width = 0;
                    int32_t height = 0;
                    int32_t channels = 0;
                    const std::string path(file_path.uri.path().begin(), file_path.uri.path().end()); // Thanks C++.

                    uint8_t *data = stbi_load(path.c_str(), &width, &height, &channels, 4);

                    std::shared_ptr<hyper_rhi::Texture> texture = graphics_device->create_texture({
                        .label = image.name,
                        .width = static_cast<uint32_t>(width),
                        .height = static_cast<uint32_t>(height),
                        .depth = 1,
                        .array_size = 1,
                        .mip_levels = 1,
                        .format = hyper_rhi::Format::Rgba8Unorm,
                        .dimension =,
                        .usage =,
                    });

                    glTextureStorage2D(texture, getLevelCount(width, height), GL_RGBA8, width, height);
                    glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

                    stbi_image_free(data);

                    m_textures.push_back(texture);
                },
                [&](fastgltf::sources::Array &vector)
                {
                    int width, height, nrChannels;
                    unsigned char *data = stbi_load_from_memory(
                        reinterpret_cast<const stbi_uc *>(vector.bytes.data()),
                        static_cast<int>(vector.bytes.size()),
                        &width,
                        &height,
                        &nrChannels,
                        4);
                    glTextureStorage2D(texture, getLevelCount(width, height), GL_RGBA8, width, height);
                    glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    stbi_image_free(data);
                },
                [&](fastgltf::sources::BufferView &view)
                {
                    auto &bufferView = viewer->asset.bufferViews[view.bufferViewIndex];
                    auto &buffer = viewer->asset.buffers[bufferView.bufferIndex];
                    // Yes, we've already loaded every buffer into some GL buffer. However, with GL it's simpler
                    // to just copy the buffer data again for the texture. Besides, this is just an example.
                    std::visit(
                        fastgltf::visitor{ // We only care about VectorWithMime here, because we specify LoadExternalBuffers, meaning
                                           // all buffers are already loaded into a vector.
                                           [](auto &arg)
                                           {
                                           },
                                           [&](fastgltf::sources::Array &vector)
                                           {
                                               int width, height, nrChannels;
                                               unsigned char *data = stbi_load_from_memory(
                                                   reinterpret_cast<const stbi_uc *>(vector.bytes.data() + bufferView.byteOffset),
                                                   static_cast<int>(bufferView.byteLength),
                                                   &width,
                                                   &height,
                                                   &nrChannels,
                                                   4);
                                               glTextureStorage2D(texture, getLevelCount(width, height), GL_RGBA8, width, height);
                                               glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
                                               stbi_image_free(data);
                                           } },
                        buffer.data);
                },
            },
            image.data);

        // glGenerateTextureMipmap(texture);
    }

    void Model::load_material(const fastgltf::Material &material)
    {
        Material material_data = {};
        material_data.alpha_cutoff = material.alphaCutoff;

        material_data.base_color_factor = glm::vec4(
            material.pbrData.baseColorFactor.x(),
            material.pbrData.baseColorFactor.y(),
            material.pbrData.baseColorFactor.z(),
            material.pbrData.baseColorFactor.w());
        if (material.pbrData.baseColorTexture.has_value())
        {
            material_data.flags |= 1;
        }

        m_materials.push_back(material_data);
    }

} // namespace hyper_render