/*
 * Copyright (c) 2023 SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "globals.hlsli"

struct Vertex {
  float4 position;
  float4 color;
  float4 uv;
  float4 unused_0;
};

struct VertexOutput {
  float4 position : SV_POSITION;
  [[vk::location(0)]] float4 color : COLOR;
  [[vk::location(1)]] float2 uv : TEXCOORD;
};

VertexOutput main(
  uint vertex_id : SV_VertexID
) {
  Frame frame = get_frame();

  GuiBindings gui = get_bindings<GuiBindings>();

  Vertex vertex = gui.get_vertex<Vertex>(vertex_id);

  VertexOutput output = (VertexOutput) 0;
  output.position = float4(
    2.0 * vertex.position.x / frame.screen_size.x - 1.0,
    2.0 * vertex.position.y / frame.screen_size.y - 1.0,
    0.0, 1.0
  );
  output.color = float4(srgb_to_linear(vertex.color.xyz), vertex.color.a);
  output.uv = vertex.uv.xy;
  return output;
}
