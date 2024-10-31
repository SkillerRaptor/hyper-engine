/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <optional>
#include <vector>

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
    public:
        [[nodiscard]] static std::optional<std::vector<std::shared_ptr<Mesh>>> load_model(
            const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device,
            const std::shared_ptr<hyper_rhi::Queue> &queue,
            const std::string &path);

    public:
        std::string name;
        std::vector<Surface> surfaces;
        std::shared_ptr<hyper_rhi::Buffer> positions;
        std::shared_ptr<hyper_rhi::Buffer> normals;
        std::shared_ptr<hyper_rhi::Buffer> colors;
        std::shared_ptr<hyper_rhi::Buffer> mesh;
        std::shared_ptr<hyper_rhi::Buffer> indices;
    };
} // namespace hyper_render