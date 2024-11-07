/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <hyper_core/math.hpp>
#include <hyper_rhi/graphics_device.hpp>

namespace hyper_render
{
    struct Surface
    {
        uint32_t start_index;
        uint32_t count;
    };

    struct Mesh
    {
        [[nodiscard]] static std::optional<std::vector<std::shared_ptr<Mesh>>>
            load_model(const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device, const std::string &path);

        std::string name;
        std::vector<Surface> surfaces;

        std::vector<glm::vec4> positions;
        std::shared_ptr<hyper_rhi::Buffer> positions_buffer;

        std::vector<glm::vec4> normals;
        std::shared_ptr<hyper_rhi::Buffer> normals_buffer;

        std::vector<glm::vec4> colors;
        std::shared_ptr<hyper_rhi::Buffer> colors_buffer;

        std::shared_ptr<hyper_rhi::Buffer> mesh_buffer;

        std::vector<uint32_t> indices;
        std::shared_ptr<hyper_rhi::Buffer> indices_buffer;
    };
} // namespace hyper_render