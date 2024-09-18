//
// Copyright (c) 2024, SkillerRaptor
//
// SPDX-License-Identifier: MIT
//

use std::{fmt::Debug, sync::Arc};

use downcast_rs::Downcast;

use crate::{pipeline_layout::PipelineLayout, shader_module::ShaderModule};

#[derive(Clone, Debug)]
pub struct GraphicsPipelineDescriptor<'a> {
    pub label: Option<&'a str>,
    pub layout: &'a Arc<dyn PipelineLayout>,
    pub vertex_shader: &'a Arc<dyn ShaderModule>,
    pub fragment_shader: &'a Arc<dyn ShaderModule>,
}

pub trait GraphicsPipeline: Debug + Downcast {}

downcast_rs::impl_downcast!(GraphicsPipeline);
