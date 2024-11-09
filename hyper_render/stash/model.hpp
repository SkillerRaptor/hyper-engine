/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

#include <fastgltf/types.hpp>

#include <hyper_rhi/command_list.hpp>
#include <hyper_rhi/graphics_device.hpp>
#include <hyper_rhi/render_pass.hpp>
#include <hyper_rhi/texture.hpp>

#include "hyper_render/mesh.hpp"

namespace hyper_render
{
    struct Material
    {
        glm::vec4 base_color_factor;
        float alpha_cutoff = 0.0;
        uint32_t flags = 0;
    };

    class Model
    {
    public:
        explicit Model(
            const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device,
            const std::shared_ptr<hyper_rhi::CommandList> &command_list,
            const std::string &path);

        void draw(const std::shared_ptr<hyper_rhi::RenderPass> &render_pass) const;

    private:
        void load_mesh(const fastgltf::Mesh &mesh);
        void load_image(const fastgltf::Image &image);
        void load_material(const fastgltf::Material &material);

    private:
        std::vector<Mesh> m_meshes;
        std::vector<std::shared_ptr<hyper_rhi::Texture>> m_textures;
        std::vector<Material> m_materials;
    };
} // namespace hyper_render