/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef HE_SHADER_INTEROP_HPP
#define HE_SHADER_INTEROP_HPP

#ifdef __cplusplus
#    include <hyper_core/math.hpp>
#    include <hyper_rhi/resource_handle.hpp>

#    define float2 ::glm::vec2
#    define float3 ::glm::vec3
#    define float4 ::glm::vec4
#    define float4x4 ::glm::mat4

#    define int2 ::glm::i32vec2
#    define int3 ::glm::i32vec3
#    define int4 ::glm::i32vec4

#    define uint uint32_t
#    define uint2 ::glm::u32vec2
#    define uint3 ::glm::u32vec3
#    define uint4 ::glm::u32vec4

#    define RESOURCE_HANDLE ::hyper_engine::ResourceHandle
#    define SIMPLE_BUFFER RESOURCE_HANDLE
#    define RW_SIMPLE_BUFFER RESOURCE_HANDLE
#    define ARRAY_BUFFER RESOURCE_HANDLE
#    define RW_ARRAY_BUFFER RESOURCE_HANDLE
#    define TEXTURE RESOURCE_HANDLE
#    define RW_TEXTURE RESOURCE_HANDLE
#    define SAMPLER RESOURCE_HANDLE
#else
#    include "globals.hlsli"

#    define RESOURCE_HANDLE uint
#    define SIMPLE_BUFFER SimpleBuffer
#    define RW_SIMPLE_BUFFER RwSimpleBuffer
#    define ARRAY_BUFFER ArrayBuffer
#    define RW_ARRAY_BUFFER RwArrayBuffer
#    define TEXTURE Texture
#    define RW_TEXTURE RwTexture
#    define SAMPLER Sampler
#endif

////////////////////////////////////////////////////////////////////////////////
// Shader Interop
////////////////////////////////////////////////////////////////////////////////

struct ShaderScene
{
    float4 ambient_color;
    float4 sunlight_direction; // .w for sun power
    float4 sunlight_color;
    float4 padding_0;
};

struct ShaderMesh
{
    ARRAY_BUFFER positions;
    ARRAY_BUFFER normals;
    ARRAY_BUFFER colors;
    ARRAY_BUFFER tex_coords;

#ifndef __cplusplus
    inline float4 get_position(uint index)
    {
        return positions.load<float4>(index);
    }

    inline float4 get_normal(uint index)
    {
        return normals.load<float4>(index);
    }

    inline float4 get_color(uint index)
    {
        return colors.load<float4>(index);
    }

    inline float4 get_tex_coord(uint index)
    {
        return tex_coords.load<float4>(index);
    }
#endif
};

struct ShaderMaterial
{
    float4 color_factors;
    TEXTURE color_texture;
    SAMPLER color_sampler;
    uint padding_0;
    uint padding_1;

    float4 metal_roughness_factors;
    TEXTURE metal_roughness_texture;
    SAMPLER metal_roughness_sampler;
    uint padding_2;
    uint padding_3;
};

////////////////////////////////////////////////////////////////////////////////
// Push Constants
////////////////////////////////////////////////////////////////////////////////

#ifndef __cplusplus
#    ifdef HE_VULKAN
#        define HE_PUSH_CONSTANT(value_type, name) [[vk::push_constant]] value_type name
#    else
#        define HE_PUSH_CONSTANT(value_type, name) ConstantBuffer<value_type> name : register(b0, space0)
#    endif
#endif

struct ObjectPushConstants
{
    SIMPLE_BUFFER scene;
    SIMPLE_BUFFER mesh;
    SIMPLE_BUFFER material;
    uint padding_0;
    float4x4 transform_matrix;

#ifndef __cplusplus
    inline ShaderScene get_scene()
    {
        return scene.load<ShaderScene>();
    }

    inline ShaderMesh get_mesh()
    {
        return mesh.load<ShaderMesh>();
    }

    inline ShaderMaterial get_material()
    {
        return material.load<ShaderMaterial>();
    }
#endif
};

////////////////////////////////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////////////////////////////////

// NOTE: This assumes we have 1000 * 1000 descriptors available. This might need refining
#define HE_DESCRIPTOR_SET_SLOT_FRAME ((1000 * 1000) - 1)
#define HE_DESCRIPTOR_SET_SLOT_CAMERA ((1000 * 1000) - 2)

struct ShaderFrame
{
    float time;
    float delta_time;
    float unused_0;
    float unused_1;

    uint frame_count;
    uint unused_2;
    uint unused_3;
    uint unused_4;

    float2 screen_size;
    float2 unused_5;
};

#ifndef __cplusplus
inline ShaderFrame get_frame()
{
    SimpleBuffer buffer = (SimpleBuffer) HE_DESCRIPTOR_SET_SLOT_FRAME;
    return buffer.load<ShaderFrame>();
}
#endif

struct ShaderCamera
{
    float4 position;

    float4x4 view;
    float4x4 inverse_view;
    float4x4 projection;
    float4x4 inverse_projection;
    float4x4 view_projection;
    float4x4 inverse_view_projection;

    float near_plane;
    float far_plane;
    float padding_0;
    float padding_1;
};

#ifndef __cplusplus
inline ShaderCamera get_camera()
{
    SimpleBuffer buffer = (SimpleBuffer) HE_DESCRIPTOR_SET_SLOT_CAMERA;
    return buffer.load<ShaderCamera>();
}
#endif

#ifdef __cplusplus
#    undef float2
#    undef float3
#    undef float4
#    undef float4x4

#    undef int2
#    undef int3
#    undef int4

#    undef uint
#    undef uint2
#    undef uint3
#    undef uint4

#    undef RESOURCE_HANDLE
#    undef ARRAY_BUFFER
#else
#    undef RESOURCE_HANDLE
#    undef ARRAY_BUFFER
#endif

#endif // HE_SHADER_INTEROP_HPP
