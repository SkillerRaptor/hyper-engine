/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

#include <hyper_core/math.hpp>
#include <hyper_rhi/graphics_device.hpp>
#include <hyper_rhi/shader_compiler.hpp>

namespace hyper_render
{
    enum class MaterialPassType : uint8_t
    {
        MainColor,
        Transparent,
    };

    struct MaterialInstance
    {
        std::shared_ptr<hyper_rhi::RenderPipeline> pipeline = nullptr;
        MaterialPassType pass_type = MaterialPassType::MainColor;

        std::shared_ptr<hyper_rhi::Buffer> buffer = nullptr;
    };

    class GltfMetallicRoughness
    {
    public:
        struct MaterialResources
        {
            glm::vec4 color_factors;
            std::shared_ptr<hyper_rhi::TextureView> color_texture_view = nullptr;
            std::shared_ptr<hyper_rhi::Sampler> color_sampler = nullptr;

            glm::vec4 metal_roughness_factors;
            std::shared_ptr<hyper_rhi::TextureView> metal_roughness_texture_view = nullptr;
            std::shared_ptr<hyper_rhi::Sampler> metal_roughness_sampler = nullptr;
        };

    public:
        GltfMetallicRoughness(
            const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device,
            const hyper_rhi::ShaderCompiler &shader_compiler,
            const std::shared_ptr<hyper_rhi::Texture> &render_texture,
            const std::shared_ptr<hyper_rhi::Texture> &depth_texture);

        MaterialInstance write_material(
            const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device,
            const std::shared_ptr<hyper_rhi::CommandList> &command_list,
            MaterialPassType pass_type,
            const MaterialResources &resources) const;

    private:
        std::shared_ptr<hyper_rhi::RenderPipeline> m_opaque_pipeline;
        std::shared_ptr<hyper_rhi::RenderPipeline> m_transparent_pipeline;
    };
} // namespace hyper_render