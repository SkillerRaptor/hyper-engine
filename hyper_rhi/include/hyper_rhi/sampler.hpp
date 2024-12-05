/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

#include "hyper_rhi/compare_operation.hpp"
#include "hyper_rhi/resource_handle.hpp"

namespace hyper_engine
{
    enum class Filter : uint8_t
    {
        Nearest,
        Linear,
    };

    enum class AddressMode : uint8_t
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
        MirrorClampToEdge
    };

    enum class BorderColor : uint8_t
    {
        TransparentBlack,
        OpaqueBlack,
        OpaqueWhite,
    };

    struct SamplerDescriptor
    {
        std::string label;

        Filter mag_filter = Filter::Linear;
        Filter min_filter = Filter::Linear;
        Filter mipmap_filter = Filter::Linear;
        AddressMode address_mode_u = AddressMode::ClampToEdge;
        AddressMode address_mode_v = AddressMode::ClampToEdge;
        AddressMode address_mode_w = AddressMode::ClampToEdge;
        float mip_lod_bias = 0.0;
        CompareOperation compare_operation = CompareOperation::Less;
        float min_lod = 0.0;
        float max_lod = 1.0;
        BorderColor border_color = BorderColor::TransparentBlack;

        ResourceHandle handle;
    };

    class ISampler
    {
    public:
        virtual ~ISampler() = default;

        virtual std::string_view label() const = 0;

        virtual Filter mag_filter() const = 0;
        virtual Filter min_filter() const = 0;
        virtual Filter mipmap_filter() const = 0;
        virtual AddressMode address_mode_u() const = 0;
        virtual AddressMode address_mode_v() const = 0;
        virtual AddressMode address_mode_w() const = 0;
        virtual float mip_lod_bias() const = 0;
        virtual CompareOperation compare_operation() const = 0;
        virtual float min_lod() const = 0;
        virtual float max_lod() const = 0;
        virtual BorderColor border_color() const = 0;

        virtual ResourceHandle handle() const = 0;
    };
} // namespace hyper_engine