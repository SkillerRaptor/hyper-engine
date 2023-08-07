/*
 * Copyright (c) 2023, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

use crate::{
    command_pool::CommandPool,
    device::Device,
    error::{CreationError, RuntimeError},
    pipeline::Pipeline,
};

use ash::vk;
use std::{mem, slice, sync::Arc};

pub(crate) struct CommandBuffer {
    handle: vk::CommandBuffer,
    device: Arc<Device>,
}

impl CommandBuffer {
    pub(crate) fn new(
        device: Arc<Device>,
        command_pool: &CommandPool,
    ) -> Result<Self, CreationError> {
        let allocate_info = vk::CommandBufferAllocateInfo::builder()
            .command_pool(command_pool.handle())
            .command_buffer_count(1)
            .level(vk::CommandBufferLevel::PRIMARY);

        let handle = unsafe {
            device
                .handle()
                .allocate_command_buffers(&allocate_info)
                .map_err(|error| CreationError::VulkanAllocation(error, "command buffer"))
        }?[0];

        Ok(Self { handle, device })
    }

    pub(crate) fn begin(
        &self,
        usage_flags: vk::CommandBufferUsageFlags,
    ) -> Result<(), RuntimeError> {
        let command_buffer_begin_info = vk::CommandBufferBeginInfo::builder().flags(usage_flags);

        unsafe {
            self.device
                .handle()
                .begin_command_buffer(self.handle, &command_buffer_begin_info)
                .map_err(RuntimeError::CommandBufferBegin)
        }?;

        Ok(())
    }

    pub(crate) fn end(&self) -> Result<(), RuntimeError> {
        unsafe {
            self.device
                .handle()
                .end_command_buffer(self.handle)
                .map_err(RuntimeError::CommandBufferEnd)
        }?;

        Ok(())
    }

    pub(crate) fn reset(&self) -> Result<(), RuntimeError> {
        unsafe {
            self.device
                .handle()
                .reset_command_buffer(self.handle, vk::CommandBufferResetFlags::from_raw(0))
                .map_err(RuntimeError::CommandBufferReset)
        }?;

        Ok(())
    }

    pub(crate) fn begin_rendering(&self, rendering_info: vk::RenderingInfo) {
        unsafe {
            self.device
                .handle()
                .cmd_begin_rendering(self.handle, &rendering_info);
        }
    }

    pub(crate) fn end_rendering(&self) {
        unsafe {
            self.device.handle().cmd_end_rendering(self.handle);
        }
    }

    pub(crate) fn bind_pipeline(
        &self,
        pipeline_bind_point: vk::PipelineBindPoint,
        pipeline: &Pipeline,
    ) {
        unsafe {
            self.device.handle().cmd_bind_pipeline(
                self.handle,
                pipeline_bind_point,
                pipeline.handle(),
            );
        }
    }

    pub(crate) fn pipeline_barrier2(&self, dependency_info: vk::DependencyInfo) {
        unsafe {
            self.device
                .handle()
                .cmd_pipeline_barrier2(self.handle, &dependency_info);
        }
    }

    pub(crate) fn set_viewport(&self, first_viewport: u32, viewports: &[vk::Viewport]) {
        unsafe {
            self.device
                .handle()
                .cmd_set_viewport(self.handle, first_viewport, viewports);
        }
    }

    pub(crate) fn set_scissor(&self, first_scissor: u32, scissors: &[vk::Rect2D]) {
        unsafe {
            self.device
                .handle()
                .cmd_set_scissor(self.handle, first_scissor, scissors);
        }
    }

    pub(crate) fn bind_descriptor_sets(
        &self,
        pipeline_bind_point: vk::PipelineBindPoint,
        pipeline: &Pipeline,
        first_set: u32,
        descriptor_sets: &[vk::DescriptorSet],
        dynamic_offsets: &[u32],
    ) {
        unsafe {
            self.device.handle().cmd_bind_descriptor_sets(
                self.handle,
                pipeline_bind_point,
                pipeline.layout(),
                first_set,
                descriptor_sets,
                dynamic_offsets,
            );
        }
    }

    pub(crate) fn push_constants<T>(
        &self,
        pipeline: &Pipeline,
        stage_flags: vk::ShaderStageFlags,
        offset: u32,
        constants: &T,
    ) {
        unsafe {
            self.device.handle().cmd_push_constants(
                self.handle,
                pipeline.layout(),
                stage_flags,
                offset,
                slice::from_raw_parts(constants as *const T as *const u8, mem::size_of::<T>()),
            );
        }
    }

    pub(crate) fn draw(
        &self,
        vertex_count: u32,
        instance_count: u32,
        first_vertex: u32,
        first_instance: u32,
    ) {
        unsafe {
            self.device.handle().cmd_draw(
                self.handle,
                vertex_count,
                instance_count,
                first_vertex,
                first_instance,
            );
        }
    }

    pub(crate) fn handle(&self) -> vk::CommandBuffer {
        self.handle
    }
}
