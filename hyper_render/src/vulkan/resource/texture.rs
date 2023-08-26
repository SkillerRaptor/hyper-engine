/*
 * Copyright (c) 2023, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

use crate::vulkan::{
    core::device::Device,
    descriptors::upload_manager::UploadManager,
    memory::allocator::{
        Allocation, AllocationCreateInfo, AllocationScheme, Allocator, MemoryLocation,
    },
    resource::buffer::{Buffer, BufferCreateInfo},
};

use ash::vk;
use color_eyre::Result;
use image::io::Reader as ImageReader;
use std::{cell::RefCell, rc::Rc};

pub(crate) struct Texture {
    allocation: Option<Allocation>,
    view: vk::ImageView,
    raw: vk::Image,

    allocator: Rc<RefCell<Allocator>>,
    device: Rc<Device>,
}

impl Texture {
    pub(crate) fn new(
        device: Rc<Device>,
        allocator: Rc<RefCell<Allocator>>,
        upload_manager: Rc<RefCell<UploadManager>>,
        file: &str,
    ) -> Result<Self> {
        let image = ImageReader::open(file)?.decode()?;

        // TODO: Don't hardcode channels
        let image_size = image.width() * image.height() * 4;

        let staging_buffer = Buffer::new(
            device.clone(),
            allocator.clone(),
            BufferCreateInfo {
                label: "Buffer Staging Texture",

                usage: vk::BufferUsageFlags::TRANSFER_SRC,
                size: image_size as u64,
                location: MemoryLocation::CpuToGpu,
            },
        )?;

        staging_buffer.set_data(image.as_bytes())?;

        let extent = vk::Extent3D::builder()
            .width(image.width())
            .height(image.height())
            .depth(1);

        let format = vk::Format::R8G8B8A8_SRGB;

        let create_info = vk::ImageCreateInfo::builder()
            .image_type(vk::ImageType::TYPE_2D)
            .format(format)
            .extent(*extent)
            .mip_levels(1)
            .array_layers(1)
            .samples(vk::SampleCountFlags::TYPE_1)
            .tiling(vk::ImageTiling::OPTIMAL)
            .usage(vk::ImageUsageFlags::SAMPLED | vk::ImageUsageFlags::TRANSFER_DST)
            .sharing_mode(vk::SharingMode::EXCLUSIVE)
            .queue_family_indices(&[])
            .initial_layout(vk::ImageLayout::UNDEFINED);

        let raw = device.create_vk_image(*create_info)?;

        let memory_requirements = device.get_image_memory_requirements(raw);

        // TODO: Add label
        let allocation = allocator.borrow_mut().allocate(AllocationCreateInfo {
            label: Some("<name> texture"),
            requirements: memory_requirements,
            location: MemoryLocation::GpuOnly,
            scheme: AllocationScheme::DedicatedImage(raw),
        })?;

        device.bind_image_memory(raw, allocation.memory(), allocation.offset())?;

        upload_manager
            .borrow_mut()
            .upload_texture(&staging_buffer, raw, *extent)?;

        let subsource_range = vk::ImageSubresourceRange::builder()
            .aspect_mask(vk::ImageAspectFlags::COLOR)
            .base_mip_level(0)
            .level_count(1)
            .base_array_layer(0)
            .layer_count(1);

        let view_create_info = vk::ImageViewCreateInfo::builder()
            .image(raw)
            .view_type(vk::ImageViewType::TYPE_2D)
            .format(format)
            .components(vk::ComponentMapping::default())
            .subresource_range(*subsource_range);

        let view = device.create_vk_image_view(*view_create_info)?;

        Ok(Self {
            allocation: Some(allocation),
            view,
            raw,

            allocator,
            device,
        })
    }

    pub(crate) fn view(&self) -> vk::ImageView {
        self.view
    }
}

impl Drop for Texture {
    fn drop(&mut self) {
        self.device.destroy_image_view(self.view);
        self.allocator
            .borrow_mut()
            .free(self.allocation.take().unwrap())
            .unwrap();

        self.device.destroy_image(self.raw);
    }
}
