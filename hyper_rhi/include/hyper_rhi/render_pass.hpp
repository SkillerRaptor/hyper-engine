/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

#include "hyper_rhi/buffer.hpp"
#include "hyper_rhi/render_pipeline.hpp"
#include "hyper_rhi/pass.hpp"
#include "hyper_rhi/texture_view.hpp"

namespace hyper_rhi
{
    enum class LoadOperation : uint8_t
    {
        Clear,
        Load,
        DontCare,
    };

    enum class StoreOperation: uint8_t
    {
        Store,
        DontCare,
    };

    struct Operations
    {
        LoadOperation load_operation = LoadOperation::Clear;
        StoreOperation store_operation = StoreOperation::Store;
    };

    struct ColorAttachment
    {
        std::shared_ptr<TextureView> view = nullptr;
        Operations operation = {};
    };

    struct DepthStencilAttachment
    {
        std::shared_ptr<TextureView> view = nullptr;
        Operations depth_operation = {};
        // TODO: Add stencil operation
    };

    struct RenderPassDescriptor
    {
        std::string label;
        LabelColor label_color;

        std::vector<ColorAttachment> color_attachments = {};
        DepthStencilAttachment depth_stencil_attachment = {};
    };

    class RenderPass : public Pass
    {
    public:
        virtual ~RenderPass() = default;

        virtual void set_pipeline(const std::shared_ptr<RenderPipeline> &pipeline) = 0;
        virtual void set_push_constants(const void *data, size_t data_size) const = 0;

        virtual void set_index_buffer(const std::shared_ptr<Buffer> &buffer) const = 0;

        virtual void set_scissor(int32_t x, int32_t y, uint32_t width, uint32_t height) const = 0;
        virtual void set_viewport(float x, float y, float width, float height, float min_depth, float max_depth) const = 0;

        virtual void draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance) const = 0;
        virtual void draw_indexed(
            uint32_t index_count,
            uint32_t instance_count,
            uint32_t first_index,
            int32_t vertex_offset,
            uint32_t first_instance) const = 0;

        // TODO: Add indirect

        [[nodiscard]] HE_FORCE_INLINE const std::vector<ColorAttachment> &color_attachments() const
        {
            return m_color_attachments;
        }

        [[nodiscard]] HE_FORCE_INLINE DepthStencilAttachment depth_stencil_attachment() const
        {
            return m_depth_stencil_attachment;
        }

    protected:
        explicit RenderPass(const RenderPassDescriptor &descriptor)
            : Pass(descriptor.label, descriptor.label_color)
              , m_color_attachments(descriptor.color_attachments)
              , m_depth_stencil_attachment(descriptor.depth_stencil_attachment)
        {
        }

    protected:
        std::vector<ColorAttachment> m_color_attachments;
        DepthStencilAttachment m_depth_stencil_attachment;
    };
} // namespace hyper_rhi