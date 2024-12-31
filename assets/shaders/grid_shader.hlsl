/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

// NOTE: Implementation based on https://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/

#include "globals.hlsli"
#include "shader_interop.h"

static float3 s_grid_plane[6] = {
    float3( 1.0,  1.0, 0.0),
    float3(-1.0, -1.0, 0.0),
    float3(-1.0,  1.0, 0.0),
    float3(-1.0, -1.0, 0.0),
    float3( 1.0,  1.0, 0.0),
    float3( 1.0, -1.0, 0.0),
};

struct VertexOutput {
    float4 position : SV_POSITION;
    float3 near_point : TEXCOORD0;
    float3 far_point : TEXCOORD1;
};

float3 unproject_point(float3 xyz, float4x4 inverse_view, float4x4 inverse_projection) {
    const float4 unprojected_point = mul(mul(inverse_view, inverse_projection), float4(xyz, 1.0));
    return unprojected_point.xyz / unprojected_point.w;
}

VertexOutput vs_main(
  uint vertex_id : SV_VertexID
) {
    const ShaderCamera camera = get_camera();

    const float3 grid_point = s_grid_plane[vertex_id];
    const float3 near_point = unproject_point(float3(grid_point.x, grid_point.y, 0.0), camera.inverse_view, camera.inverse_projection);
    const float3 far_point = unproject_point(float3(grid_point.x, grid_point.y, 1.0), camera.inverse_view, camera.inverse_projection);

    VertexOutput output = (VertexOutput) 0;
    output.position = float4(s_grid_plane[vertex_id], 1.0);
    output.near_point = near_point;
    output.far_point = far_point;
    return output;
}

struct FragmentOutput {
    float4 color : SV_TARGET;
    float depth : SV_DEPTH;
};

float4 grid(float3 fragment_position, float scale) {
    const float2 coordinate = fragment_position.xz * scale;
    const float2 derivative = fwidth(coordinate);
    const float2 grid = abs(frac(coordinate - 0.5) - 0.5) / derivative;
    const float l = min(grid.x, grid.y);
    const float minimum_z = min(derivative.y, 1.0);
    const float minimum_x = min(derivative.x, 1.0);

    float4 color = float4(0.2, 0.2, 0.2, 1.0 - min(l, 1.0));
    if (fragment_position.x > -0.1 * minimum_x && fragment_position.x < 0.1 * minimum_x) {
        color.b = 1.0;
    }

    if (fragment_position.z > -0.1 * minimum_z && fragment_position.z < 0.1 * minimum_z) {
        color.r = 1.0;
    }

    return color;
}

float compute_depth(float3 position, float4x4 view_projection) {
    const float4 clip_space_position = mul(view_projection, float4(position.xyz, 1.0));
    return (clip_space_position.z / clip_space_position.w);
}

float compute_linear_depth(float3 position, float4x4 view_projection, float near_plane, float far_plane) {
    const float4 clip_space_position = mul(view_projection, float4(position.xyz, 1.0));
    const float clip_space_depth = (clip_space_position.z / clip_space_position.w) * 2.0 - 1.0;
    const float linear_depth = (2.0 * near_plane * far_plane) / (far_plane + near_plane - clip_space_depth * (far_plane - near_plane));
    return linear_depth / far_plane;
}

FragmentOutput fs_main(VertexOutput input) {
    const ShaderCamera camera = get_camera();

    const float t = -input.near_point.y / (input.far_point.y - input.near_point.y);
    const float3 fragment_position = input.near_point + t * (input.far_point - input.near_point);

    const float linear_depth = compute_linear_depth(fragment_position, camera.view_projection, camera.near_plane, camera.far_plane);
    const float fading = max(0.0, (0.5 - linear_depth));

    float4 color = (grid(fragment_position, 10.0) + grid(fragment_position, 1.0)) * float(t > 0.0);
    color.a *= fading;

    FragmentOutput output = (FragmentOutput) 0;
    output.color = color;
    output.depth = compute_depth(fragment_position, camera.view_projection);

    return output;
}
