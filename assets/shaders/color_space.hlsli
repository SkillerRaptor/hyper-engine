/*
 * Copyright (c) 2024 SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

float3 apply_srgb(float3 x) {
    return pow(x, 1.0 / 2.2);
}

float3 remove_srgb(float3 x) {
    return pow(x, 2.2);
}
