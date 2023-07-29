/*
 * Copyright (c) 2023, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

use crate::{
    command_buffer::CommandBuffer, descriptor_pool::DescriptorPool, device::Device,
    error::CreationError, swapchain::Swapchain,
};

use ash::vk;
use std::{
    ffi::CStr,
    fmt::{self, Display, Formatter},
    fs::File,
    io::{BufReader, Read},
    mem,
    sync::Arc,
};

#[repr(C)]
struct BindingsOffset {
    bindings_offset: u32,
    unused_0: u32,
    unused_1: u32,
    unused_2: u32,
}

#[derive(Clone, Copy, Debug)]
pub enum ShaderType {
    Fragment,
    Vertex,
}

impl Display for ShaderType {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        match *self {
            ShaderType::Fragment => write!(f, "fragment"),
            ShaderType::Vertex => write!(f, "vertex"),
        }
    }
}

pub(crate) struct Pipeline {
    layout: vk::PipelineLayout,
    handle: vk::Pipeline,

    device: Arc<Device>,
}

impl Pipeline {
    pub(crate) fn new(
        device: Arc<Device>,
        swapchain: &Swapchain,
        descriptor_pool: &DescriptorPool,
    ) -> Result<Self, CreationError> {
        let layout = Self::create_graphics_pipeline_layout(&device, descriptor_pool)?;
        let handle = Self::create_graphics_pipeline(&device, swapchain, &layout)?;

        Ok(Self {
            layout,
            handle,

            device,
        })
    }

    fn create_graphics_pipeline_layout(
        device: &Device,
        descriptor_pool: &DescriptorPool,
    ) -> Result<vk::PipelineLayout, CreationError> {
        let push_constant_size = mem::size_of::<BindingsOffset>() as u32;

        let push_constant_range = vk::PushConstantRange::builder()
            .stage_flags(vk::ShaderStageFlags::ALL)
            .offset(0)
            .size(push_constant_size);

        let push_constant_ranges = [*push_constant_range];

        let create_info = vk::PipelineLayoutCreateInfo::builder()
            .set_layouts(descriptor_pool.layouts())
            .push_constant_ranges(&push_constant_ranges);

        let handle = unsafe {
            device
                .handle()
                .create_pipeline_layout(&create_info, None)
                .map_err(|error| CreationError::VulkanCreation(error, "pipeline layout"))
        }?;

        Ok(handle)
    }

    fn create_graphics_pipeline(
        device: &Device,
        swapchain: &Swapchain,
        pipeline_layout: &vk::PipelineLayout,
    ) -> Result<vk::Pipeline, CreationError> {
        let vertex_shader_code = Self::parse_shader("./assets/shaders/compiled/triangle_vert.spv")?;
        let fragment_shader_code =
            Self::parse_shader("./assets/shaders/compiled/triangle_frag.spv")?;

        let vertex_shader_module =
            Self::create_shader_module(device, ShaderType::Vertex, &vertex_shader_code)?;
        let fragment_shader_module =
            Self::create_shader_module(device, ShaderType::Fragment, &fragment_shader_code)?;

        let entry_name = unsafe { CStr::from_bytes_with_nul_unchecked(b"main\0") };

        let vertex_create_info = vk::PipelineShaderStageCreateInfo::builder()
            .stage(vk::ShaderStageFlags::VERTEX)
            .module(vertex_shader_module)
            .name(entry_name);

        let fragment_create_info = vk::PipelineShaderStageCreateInfo::builder()
            .stage(vk::ShaderStageFlags::FRAGMENT)
            .module(fragment_shader_module)
            .name(entry_name);

        let shader_stages = [*vertex_create_info, *fragment_create_info];

        let dynamic_states = [vk::DynamicState::VIEWPORT, vk::DynamicState::SCISSOR];

        let dynamic_state_create_info =
            vk::PipelineDynamicStateCreateInfo::builder().dynamic_states(&dynamic_states);

        let vertex_input_state_create_info = vk::PipelineVertexInputStateCreateInfo::builder()
            .vertex_binding_descriptions(&[])
            .vertex_attribute_descriptions(&[]);

        let input_assembly_state_create_info = vk::PipelineInputAssemblyStateCreateInfo::builder()
            .topology(vk::PrimitiveTopology::TRIANGLE_LIST)
            .primitive_restart_enable(false);

        let viewport_state_create_info = vk::PipelineViewportStateCreateInfo::builder()
            .viewport_count(1)
            .scissor_count(1);

        let rasterization_state_create_info = vk::PipelineRasterizationStateCreateInfo::builder()
            .depth_clamp_enable(false)
            .rasterizer_discard_enable(false)
            .polygon_mode(vk::PolygonMode::FILL)
            .line_width(1.0)
            .cull_mode(vk::CullModeFlags::BACK)
            .front_face(vk::FrontFace::CLOCKWISE)
            .depth_bias_enable(false)
            .depth_bias_constant_factor(0.0)
            .depth_bias_clamp(0.0)
            .depth_bias_slope_factor(0.0);

        let multisample_state_create_info = vk::PipelineMultisampleStateCreateInfo::builder()
            .sample_shading_enable(false)
            .rasterization_samples(vk::SampleCountFlags::TYPE_1)
            .min_sample_shading(1.0)
            .sample_mask(&[])
            .alpha_to_coverage_enable(false)
            .alpha_to_one_enable(false);

        let color_blend_attachment_state = vk::PipelineColorBlendAttachmentState::builder()
            .color_write_mask(vk::ColorComponentFlags::RGBA)
            .blend_enable(false)
            .src_color_blend_factor(vk::BlendFactor::ONE)
            .dst_color_blend_factor(vk::BlendFactor::ZERO)
            .color_blend_op(vk::BlendOp::ADD)
            .src_alpha_blend_factor(vk::BlendFactor::ONE)
            .dst_alpha_blend_factor(vk::BlendFactor::ZERO)
            .alpha_blend_op(vk::BlendOp::ADD);

        let attachments = &[*color_blend_attachment_state];

        let color_blend_state_create_info = vk::PipelineColorBlendStateCreateInfo::builder()
            .logic_op_enable(false)
            .logic_op(vk::LogicOp::COPY)
            .attachments(attachments)
            .blend_constants([0.0; 4]);

        let color_attachment_formats = &[*swapchain.format()];
        let mut rendering_create_info = vk::PipelineRenderingCreateInfoKHR::builder()
            .color_attachment_formats(color_attachment_formats);

        let create_info = vk::GraphicsPipelineCreateInfo::builder()
            .push_next(&mut rendering_create_info)
            .stages(&shader_stages)
            .vertex_input_state(&vertex_input_state_create_info)
            .input_assembly_state(&input_assembly_state_create_info)
            .viewport_state(&viewport_state_create_info)
            .rasterization_state(&rasterization_state_create_info)
            .multisample_state(&multisample_state_create_info)
            .color_blend_state(&color_blend_state_create_info)
            .dynamic_state(&dynamic_state_create_info)
            .layout(*pipeline_layout)
            .render_pass(vk::RenderPass::null())
            .subpass(0)
            .base_pipeline_handle(vk::Pipeline::null())
            .base_pipeline_index(-1);

        let handles = unsafe {
            device
                .handle()
                .create_graphics_pipelines(vk::PipelineCache::null(), &[*create_info], None)
                .map_err(|error| CreationError::VulkanCreation(error.1, "graphics pipeline"))
        }?;

        unsafe {
            device
                .handle()
                .destroy_shader_module(vertex_shader_module, None);
            device
                .handle()
                .destroy_shader_module(fragment_shader_module, None);
        }

        Ok(handles[0])
    }

    fn parse_shader(file_path: &str) -> Result<Vec<u8>, CreationError> {
        let file = File::open(file_path)
            .map_err(|error| CreationError::OpenFailure(error, file_path.to_string()))?;
        let mut reader = BufReader::new(file);

        let mut buffer = Vec::new();
        reader
            .read_to_end(&mut buffer)
            .map_err(|error| CreationError::ReadFailure(error, file_path.to_string()))?;

        Ok(buffer)
    }

    fn create_shader_module(
        device: &Device,
        shader_type: ShaderType,
        shader_bytes: &[u8],
    ) -> Result<vk::ShaderModule, CreationError> {
        let (prefix, code, suffix) = unsafe { shader_bytes.align_to::<u32>() };
        if !prefix.is_empty() || !suffix.is_empty() {
            return Err(CreationError::Unaligned(shader_type));
        }

        let create_info = vk::ShaderModuleCreateInfo::builder().code(code);

        let handle = unsafe {
            device
                .handle()
                .create_shader_module(&create_info, None)
                .map_err(|error| {
                    let error_message = match shader_type {
                        ShaderType::Fragment => "fragment shader module",
                        ShaderType::Vertex => "vertex shader module",
                    };

                    CreationError::VulkanCreation(error, error_message)
                })
        }?;

        Ok(handle)
    }

    pub(crate) fn begin_rendering(
        &self,
        swapchain: &Swapchain,
        command_buffer: &CommandBuffer,
        image: &vk::Image,
        image_view: &vk::ImageView,
        clear_value: vk::ClearValue,
    ) {
        let subresource_range = vk::ImageSubresourceRange::builder()
            .aspect_mask(vk::ImageAspectFlags::COLOR)
            .base_mip_level(0)
            .level_count(1)
            .base_array_layer(0)
            .layer_count(1);

        let image_memory_barrier = vk::ImageMemoryBarrier2::builder()
            .src_stage_mask(vk::PipelineStageFlags2::TOP_OF_PIPE)
            .dst_stage_mask(vk::PipelineStageFlags2::COLOR_ATTACHMENT_OUTPUT)
            .dst_access_mask(vk::AccessFlags2::COLOR_ATTACHMENT_WRITE)
            .old_layout(vk::ImageLayout::UNDEFINED)
            .new_layout(vk::ImageLayout::COLOR_ATTACHMENT_OPTIMAL)
            .src_queue_family_index(0)
            .dst_queue_family_index(0)
            .image(*image)
            .subresource_range(*subresource_range);

        let image_memory_barriers = [*image_memory_barrier];
        let dependency_info = vk::DependencyInfo::builder()
            .dependency_flags(vk::DependencyFlags::empty())
            .memory_barriers(&[])
            .buffer_memory_barriers(&[])
            .image_memory_barriers(&image_memory_barriers);

        unsafe {
            self.device
                .handle()
                .cmd_pipeline_barrier2(*command_buffer.handle(), &dependency_info);
        }

        let render_area_extent = swapchain.extent();
        let render_area_offset = vk::Offset2D::builder().x(0).y(0);

        let render_area = vk::Rect2D::builder()
            .extent(*render_area_extent)
            .offset(*render_area_offset);

        let color_attachment_info = vk::RenderingAttachmentInfoKHR::builder()
            .image_view(*image_view)
            .image_layout(vk::ImageLayout::ATTACHMENT_OPTIMAL_KHR)
            .load_op(vk::AttachmentLoadOp::CLEAR)
            .store_op(vk::AttachmentStoreOp::STORE)
            .clear_value(clear_value);

        let color_attachments = &[*color_attachment_info];
        let rendering_info = vk::RenderingInfoKHR::builder()
            .render_area(*render_area)
            .layer_count(1)
            .color_attachments(color_attachments);

        unsafe {
            self.device
                .handle()
                .cmd_begin_rendering(*command_buffer.handle(), &rendering_info);
        }
    }

    pub(crate) fn end_rendering(&self, command_buffer: &CommandBuffer, image: &vk::Image) {
        unsafe {
            self.device
                .handle()
                .cmd_end_rendering(*command_buffer.handle());
        }

        let subresource_range = vk::ImageSubresourceRange::builder()
            .aspect_mask(vk::ImageAspectFlags::COLOR)
            .base_mip_level(0)
            .level_count(1)
            .base_array_layer(0)
            .layer_count(1);

        let image_memory_barrier = vk::ImageMemoryBarrier2::builder()
            .src_stage_mask(vk::PipelineStageFlags2::COLOR_ATTACHMENT_OUTPUT)
            .src_access_mask(vk::AccessFlags2::COLOR_ATTACHMENT_WRITE)
            .dst_stage_mask(vk::PipelineStageFlags2::BOTTOM_OF_PIPE)
            .old_layout(vk::ImageLayout::COLOR_ATTACHMENT_OPTIMAL)
            .new_layout(vk::ImageLayout::PRESENT_SRC_KHR)
            .src_queue_family_index(0)
            .dst_queue_family_index(0)
            .image(*image)
            .subresource_range(*subresource_range);

        let image_memory_barriers = [*image_memory_barrier];

        let dependency_info = vk::DependencyInfo::builder()
            .dependency_flags(vk::DependencyFlags::empty())
            .memory_barriers(&[])
            .buffer_memory_barriers(&[])
            .image_memory_barriers(&image_memory_barriers);

        unsafe {
            self.device
                .handle()
                .cmd_pipeline_barrier2(*command_buffer.handle(), &dependency_info);
        }
    }

    pub(crate) fn bind(&self, command_buffer: &CommandBuffer) {
        unsafe {
            self.device.handle().cmd_bind_pipeline(
                *command_buffer.handle(),
                vk::PipelineBindPoint::GRAPHICS,
                self.handle,
            )
        }
    }
}

impl Drop for Pipeline {
    fn drop(&mut self) {
        unsafe {
            self.device.handle().destroy_pipeline(self.handle, None);

            self.device
                .handle()
                .destroy_pipeline_layout(self.layout, None);
        }
    }
}
