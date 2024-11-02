/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_rhi/graphics_device.hpp>
#include <hyper_rhi/shader_compiler.hpp>

namespace hyper_render
{
    class GridPass
    {
    public:
        GridPass(const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device, const hyper_rhi::ShaderCompiler &shader_compiler);

        void render(
            const std::shared_ptr<hyper_rhi::CommandList> &command_list,
            const std::shared_ptr<hyper_rhi::Texture> &swapchain_texture,
            const std::shared_ptr<hyper_rhi::Texture> &depth_texture) const;

    private:
        std::shared_ptr<hyper_rhi::PipelineLayout> m_pipeline_layout;
        std::shared_ptr<hyper_rhi::ShaderModule> m_vertex_shader;
        std::shared_ptr<hyper_rhi::ShaderModule> m_fragment_shader;
        std::shared_ptr<hyper_rhi::GraphicsPipeline> m_pipeline;
    };
} // namespace hyper_render