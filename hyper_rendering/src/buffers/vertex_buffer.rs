/*
 * Copyright (c) 2022, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

use crate::{
    allocator::{Allocator, MemoryLocation},
    vertex::Vertex,
};

use hyper_core::ignore::Ignore;

use ash::{
    vk::{Buffer, BufferCreateInfo, BufferUsageFlags, SharingMode},
    Device,
};
use gpu_allocator::vulkan::Allocation;
use log::debug;
use std::{cell::RefCell, mem, rc::Rc};

pub(crate) struct VertexBufferCreateInfo<'a> {
    pub logical_device: &'a Device,
    pub allocator: &'a Rc<RefCell<Allocator>>,
    pub vertices: &'a [Vertex],
}

pub(crate) struct VertexBuffer {
    internal_buffer: Buffer,
    allocation: Option<Allocation>,

    allocator: Rc<RefCell<Allocator>>,
    logical_device: Device,
}

impl VertexBuffer {
    pub fn new(create_info: &VertexBufferCreateInfo) -> Self {
        let internal_buffer =
            Self::create_internal_buffer(create_info.logical_device, create_info.vertices.len());
        let allocation = Self::allocate_memory(
            create_info.logical_device,
            create_info.allocator,
            create_info.vertices,
            &internal_buffer,
        );

        debug!("Created vertex buffer");

        Self {
            internal_buffer,
            allocation: Some(allocation),

            allocator: create_info.allocator.clone(),
            logical_device: create_info.logical_device.clone(),
        }
    }

    fn create_internal_buffer(logical_device: &Device, vertex_count: usize) -> Buffer {
        let buffer_create_info = BufferCreateInfo::builder()
            .size((mem::size_of::<Vertex>() * vertex_count) as u64)
            .usage(BufferUsageFlags::VERTEX_BUFFER)
            .sharing_mode(SharingMode::EXCLUSIVE)
            .queue_family_indices(&[]);

        // TODO: Propagate error
        let internal_buffer = unsafe {
            logical_device
                .create_buffer(&buffer_create_info, None)
                .expect("FIXME")
        };

        debug!("Created internal vertex buffer");

        internal_buffer
    }

    fn allocate_memory(
        logical_device: &Device,
        allocator: &Rc<RefCell<Allocator>>,
        vertices: &[Vertex],
        internal_buffer: &Buffer,
    ) -> Allocation {
        let memory_requirements =
            unsafe { logical_device.get_buffer_memory_requirements(*internal_buffer) };

        // TODO: Propagate error
        let allocation = allocator
            .borrow_mut()
            .allocate(memory_requirements, MemoryLocation::CpuToGpu)
            .expect("FIXME");

        debug!("Allocated vertex buffer memory");

        // TODO: Propagate error
        unsafe {
            logical_device
                .bind_buffer_memory(*internal_buffer, allocation.memory(), allocation.offset())
                .expect("FIXME");
        }

        // TODO: Propagate error
        let memory = allocation.mapped_ptr().expect("FIXME").as_ptr() as *mut Vertex;

        unsafe {
            memory.copy_from_nonoverlapping(vertices.as_ptr(), vertices.len());
        }

        debug!("Mapped vertex buffer memory");

        allocation
    }

    pub fn internal_buffer(&self) -> &Buffer {
        &self.internal_buffer
    }
}

impl Drop for VertexBuffer {
    fn drop(&mut self) {
        // Frees vertex buffer allocation
        self.allocator
            .borrow_mut()
            // Uses unwrap because it is guaranteed that the allocation exists
            .free(self.allocation.take().unwrap())
            .ignore();

        unsafe {
            self.logical_device
                .destroy_buffer(self.internal_buffer, None);
        }
    }
}
