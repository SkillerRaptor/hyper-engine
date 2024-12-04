/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

#include <hyper_core/math.hpp>

namespace hyper_engine
{
    class IBuffer;
    class ICommandList;
    class IGraphicsDevice;
    class IRenderPipeline;
    class ISampler;
    class ShaderCompiler;
    class ITexture;
    class ITextureView;

    enum class MaterialPassType : uint8_t
    {
        MainColor,
        Transparent,
    };

    struct MaterialInstance
    {
        std::shared_ptr<IRenderPipeline> pipeline = nullptr;
        MaterialPassType pass_type = MaterialPassType::MainColor;

        std::shared_ptr<IBuffer> buffer = nullptr;
    };

    class GltfMetallicRoughness
    {
    public:
        struct MaterialResources
        {
            glm::vec4 color_factors;
            std::shared_ptr<ITextureView> color_texture_view = nullptr;
            std::shared_ptr<ISampler> color_sampler = nullptr;

            glm::vec4 metal_roughness_factors;
            std::shared_ptr<ITextureView> metal_roughness_texture_view = nullptr;
            std::shared_ptr<ISampler> metal_roughness_sampler = nullptr;
        };

    public:
        GltfMetallicRoughness(
            const std::shared_ptr<IGraphicsDevice> &graphics_device,
            const ShaderCompiler &shader_compiler,
            const std::shared_ptr<ITexture> &render_texture,
            const std::shared_ptr<ITexture> &depth_texture);

        MaterialInstance write_material(
            const std::shared_ptr<IGraphicsDevice> &graphics_device,
            const std::shared_ptr<ICommandList> &command_list,
            MaterialPassType pass_type,
            const MaterialResources &resources) const;

    private:
        std::shared_ptr<IRenderPipeline> m_opaque_pipeline;
        std::shared_ptr<IRenderPipeline> m_transparent_pipeline;
    };
} // namespace hyper_engine