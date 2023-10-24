/*
 * Copyright (c) 2023, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

pub trait Game {
    fn update(&mut self);

    fn update_fixed(&mut self, delta_time: f32, time: f32);

    fn render(&mut self);
}
