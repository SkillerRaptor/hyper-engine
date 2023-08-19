/*
 * Copyright (c) 2023, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

use crate::{
    bindings::BindingsOffset,
    vulkan::{core::device::Device, descriptors::descriptor_manager::DescriptorManager},
};

use ash::vk;
use color_eyre::Result;
use std::{mem, rc::Rc};

pub(crate) struct PipelineLayout {
    handle: vk::PipelineLayout,

    device: Rc<Device>,
}

impl PipelineLayout {
    pub(crate) fn new(device: Rc<Device>, create_info: PipelineLayoutCreateInfo) -> Result<Self> {
        let PipelineLayoutCreateInfo {
            label,

            descriptor_manager,
        } = create_info;

        let bindings_offset_size = mem::size_of::<BindingsOffset>() as u32;

        let bindings_range = vk::PushConstantRange::builder()
            .stage_flags(vk::ShaderStageFlags::ALL)
            .offset(0)
            .size(bindings_offset_size);

        let push_ranges = [*bindings_range];
        let create_info = vk::PipelineLayoutCreateInfo::builder()
            .set_layouts(descriptor_manager.descriptor_pool().layouts())
            .push_constant_ranges(&push_ranges);

        let handle = unsafe { device.handle().create_pipeline_layout(&create_info, None)? };

        device.set_object_name(vk::ObjectType::PIPELINE_LAYOUT, handle, label)?;

        Ok(Self { handle, device })
    }

    pub(crate) fn handle(&self) -> vk::PipelineLayout {
        self.handle
    }
}

impl Drop for PipelineLayout {
    fn drop(&mut self) {
        unsafe {
            self.device
                .handle()
                .destroy_pipeline_layout(self.handle, None);
        }
    }
}

pub(crate) struct PipelineLayoutCreateInfo<'a> {
    pub(crate) label: &'a str,

    pub(crate) descriptor_manager: &'a DescriptorManager,
}
