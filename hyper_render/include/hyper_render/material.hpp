/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/math.hpp>
#include <hyper_core/nonnull_ref_ptr.hpp>
#include <hyper_core/ref_ptr.hpp>
#include <hyper_rhi/forward.hpp>
#include <hyper_rhi/render_pipeline.hpp>

namespace hyper_engine
{
    enum class MaterialPassType : uint8_t
    {
        MainColor,
        Transparent,
    };

    struct MaterialInstance
    {
        RefPtr<RenderPipeline> pipeline = nullptr;
        MaterialPassType pass_type = MaterialPassType::MainColor;

        RefPtr<Buffer> buffer = nullptr;
    };

    class GltfMetallicRoughness
    {
    public:
        struct MaterialResources
        {
            glm::vec4 color_factors = {0.0f, 0.0f, 0.0f, 0.0f};
            NonnullRefPtr<TextureView> color_texture_view;
            NonnullRefPtr<Sampler> color_sampler;

            glm::vec4 metal_roughness_factors = {0.0f, 0.0f, 0.0f, 0.0f};
            NonnullRefPtr<TextureView> metal_roughness_texture_view;
            NonnullRefPtr<Sampler> metal_roughness_sampler;
        };

    public:
        GltfMetallicRoughness(
            const ShaderCompiler &shader_compiler,
            const NonnullRefPtr<Texture> &render_texture,
            const NonnullRefPtr<Texture> &depth_texture);

        MaterialInstance
            write_material(const NonnullRefPtr<CommandList> &command_list, MaterialPassType pass_type, const MaterialResources &resources) const;

    private:
        // FIXME: Make this NonnullRefPtr by using a factory function
        RefPtr<RenderPipeline> m_opaque_pipeline = nullptr;
        RefPtr<RenderPipeline> m_transparent_pipeline = nullptr;
    };
} // namespace hyper_engine