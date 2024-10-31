/*
* Copyright (c) 2024 SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "globals.hlsli"
#include "shader_interop.h"

HE_PUSH_CONSTANT(ObjectPushConstants, g_push);

struct VertexOutput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VertexOutput vs_main(
  uint vertex_id : SV_VertexID
) {
    const Mesh mesh = g_push.get_mesh();
    const float4 position = mesh.get_position(vertex_id) + float4(0.0, 0.0, 1.0, 0.0);
    const float4 normal = mesh.get_normal(vertex_id);
    const float4 color = mesh.get_color(vertex_id);

    const Material material = g_push.get_material();
    const float4 base_color = material.base_color;

    const Camera camera = get_camera();

    VertexOutput output = (VertexOutput) 0;
    output.position = mul(camera.view_projection, position);
    output.color = float4(normal.xyz * 0.5 + 0.5, 1.0);
    return output;
}

float4 fs_main(VertexOutput input) : SV_TARGET {
    return input.color;
}
