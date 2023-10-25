/*
 * Copyright (c) 2023 SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "globals.hlsli"

struct FragmentInput {
  [[vk::location(0)]] float4 color : COLOR;
  [[vk::location(1)]] float2 tex_coord : TEXCOORD;
};

float4 main(FragmentInput input) : SV_TARGET {
  Scene scene = get_scene();

  ObjectBindings object = get_bindings<ObjectBindings>();
  ObjectMaterial material = object.get_material(scene);

  Texture texture = material.get_texture(OBJECT_TEXTURE_SLOT_ALBEDO_MAP);
  float4 value = texture.sample_2d<float4>(input.tex_coord);
  if (value.a < 0.1) {
    discard;
  }
  return value;
}
