/*
 * Copyright (c) 2023, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

pub trait Handle32 {
    const BIT_COUNT: u32 = u32::BITS;
    const SHIFT_COUNT: u32 = u32::BITS / 2;

    const HANDLE_MASK: u32 = (1 << Self::SHIFT_COUNT) - 1;
    const VERSION_MASK: u32 = Self::HANDLE_MASK << Self::SHIFT_COUNT;

    fn increase_version(&mut self) {
        let current_version = self.version() as u32;
        *self.value_mut() =
            ((current_version + 1) << Self::SHIFT_COUNT) | (self.value() & Self::HANDLE_MASK);
    }

    fn swap_handle(&mut self, handle: &mut u16) {
        let current_handle_id = self.handle();
        *self.value_mut() = (self.value() & Self::VERSION_MASK) | (*handle as u32);
        *handle = current_handle_id;
    }

    fn handle(&self) -> u16 {
        (self.value() & Self::HANDLE_MASK) as u16
    }

    fn version(&self) -> u16 {
        ((self.value() & Self::VERSION_MASK) >> Self::SHIFT_COUNT) as u16
    }

    fn value(&self) -> u32;

    fn value_mut(&mut self) -> &mut u32;
}

pub trait Handle64 {
    const BIT_COUNT: u32 = u64::BITS;
    const SHIFT_COUNT: u32 = u64::BITS / 2;

    const HANDLE_MASK: u64 = (1 << Self::SHIFT_COUNT) - 1;
    const VERSION_MASK: u64 = Self::HANDLE_MASK << Self::SHIFT_COUNT;

    fn increase_version(&mut self) {
        let current_version = self.version() as u64;
        *self.value_mut() =
            ((current_version + 1) << Self::SHIFT_COUNT) | (self.value() & Self::HANDLE_MASK);
    }

    fn swap_handle(&mut self, handle: &mut u32) {
        let current_handle_id = self.handle();
        *self.value_mut() = (self.value() & Self::VERSION_MASK) | (*handle as u64);
        *handle = current_handle_id;
    }

    fn handle(&self) -> u32 {
        (self.value() & Self::HANDLE_MASK) as u32
    }

    fn version(&self) -> u32 {
        ((self.value() & Self::VERSION_MASK) >> Self::SHIFT_COUNT) as u32
    }

    fn value(&self) -> u64;

    fn value_mut(&mut self) -> &mut u64;
}
