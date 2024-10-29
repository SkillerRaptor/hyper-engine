/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

#include "hyper_rhi/buffer.hpp"
#include "hyper_rhi/graphics_pipeline.hpp"
#include "hyper_rhi/resource.hpp"
#include "hyper_rhi/texture.hpp"

namespace hyper_rhi
{
    struct DrawIndexedArguments
    {
        uint32_t index_count;
        uint32_t instance_count;
        uint32_t first_index;
        int32_t vertex_offset;
        uint32_t first_instance;
    };

    struct RenderPassDescriptor
    {
        std::string label;

        TextureHandle color_attachment = nullptr;
    };

    class RenderPass : public Resource
    {
    public:
        virtual ~RenderPass() = default;

        virtual void set_pipeline(const GraphicsPipelineHandle &pipeline_handle) = 0;
        virtual void set_index_buffer(BufferHandle buffer_handle) const = 0;
        virtual void set_push_constants(const void *data, size_t data_size) const = 0;

        virtual void draw_indexed(const DrawIndexedArguments &arguments) const = 0;

    protected:
        explicit RenderPass(std::string label);
    };

    using RenderPassHandle = std::shared_ptr<RenderPass>;
} // namespace hyper_rhi
