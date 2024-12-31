/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

#include <hyper_core/math.hpp>

namespace hyper_engine
{
    class Buffer;
    class CommandList;
    class GraphicsDevice;
    class RenderPipeline;
    class Sampler;
    class ShaderCompiler;
    class Texture;
    class TextureView;

    enum class MaterialPassType : uint8_t
    {
        MainColor,
        Transparent,
    };

    struct MaterialInstance
    {
        std::shared_ptr<RenderPipeline> pipeline = nullptr;
        MaterialPassType pass_type = MaterialPassType::MainColor;

        std::shared_ptr<Buffer> buffer = nullptr;
    };

    class GltfMetallicRoughness
    {
    public:
        struct MaterialResources
        {
            glm::vec4 color_factors;
            std::shared_ptr<TextureView> color_texture_view = nullptr;
            std::shared_ptr<Sampler> color_sampler = nullptr;

            glm::vec4 metal_roughness_factors;
            std::shared_ptr<TextureView> metal_roughness_texture_view = nullptr;
            std::shared_ptr<Sampler> metal_roughness_sampler = nullptr;
        };

    public:
        GltfMetallicRoughness(
            const ShaderCompiler &shader_compiler,
            const std::shared_ptr<Texture> &render_texture,
            const std::shared_ptr<Texture> &depth_texture);

        MaterialInstance write_material(
            const std::shared_ptr<CommandList> &command_list,
            MaterialPassType pass_type,
            const MaterialResources &resources) const;

    private:
        std::shared_ptr<RenderPipeline> m_opaque_pipeline;
        std::shared_ptr<RenderPipeline> m_transparent_pipeline;
    };
} // namespace hyper_engine