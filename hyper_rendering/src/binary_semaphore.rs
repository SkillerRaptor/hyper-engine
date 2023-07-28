/*
 * Copyright (c) 2023, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

use crate::{device::Device, error::CreationError};

use ash::vk::{Semaphore, SemaphoreCreateFlags, SemaphoreCreateInfo};
use std::sync::Arc;

pub(crate) struct BinarySemaphore {
    handle: Semaphore,
    device: Arc<Device>,
}

impl BinarySemaphore {
    pub(crate) fn new(device: Arc<Device>) -> Result<Self, CreationError> {
        let create_info = SemaphoreCreateInfo::builder().flags(SemaphoreCreateFlags::empty());

        let handle = unsafe {
            device
                .handle()
                .create_semaphore(&create_info, None)
                .map_err(|error| CreationError::VulkanCreation(error, "binary semaphore"))
        }?;

        Ok(Self { handle, device })
    }

    pub(crate) fn handle(&self) -> &Semaphore {
        &self.handle
    }
}

impl Drop for BinarySemaphore {
    fn drop(&mut self) {
        unsafe {
            self.device.handle().destroy_semaphore(self.handle, None);
        }
    }
}
