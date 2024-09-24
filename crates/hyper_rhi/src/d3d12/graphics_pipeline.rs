//
// Copyright (c) 2024, SkillerRaptor
//
// SPDX-License-Identifier: MIT
//

use std::{
    ffi::c_void,
    fmt::{self, Debug, Formatter},
    mem::{self, ManuallyDrop},
    sync::Arc,
};

use windows::Win32::Graphics::{
    Direct3D12::{
        ID3D12PipelineState,
        D3D12_BLEND_DESC,
        D3D12_BLEND_ONE,
        D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ZERO,
        D3D12_COLOR_WRITE_ENABLE_ALL,
        D3D12_CULL_MODE_NONE,
        D3D12_DEPTH_STENCIL_DESC,
        D3D12_FILL_MODE_SOLID,
        D3D12_GRAPHICS_PIPELINE_STATE_DESC,
        D3D12_LOGIC_OP_NOOP,
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
        D3D12_RASTERIZER_DESC,
        D3D12_RENDER_TARGET_BLEND_DESC,
        D3D12_SHADER_BYTECODE,
    },
    Dxgi::Common::{DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SAMPLE_DESC},
};

use crate::{
    d3d12::{
        graphics_device::GraphicsDeviceShared,
        pipeline_layout::PipelineLayout,
        shader_module::ShaderModule,
    },
    graphics_pipeline::GraphicsPipelineDescriptor,
};

pub struct GraphicsPipeline {
    raw: ManuallyDrop<ID3D12PipelineState>,

    layout: Arc<dyn crate::pipeline_layout::PipelineLayout>,

    graphics_device: Arc<GraphicsDeviceShared>,
}

impl GraphicsPipeline {
    pub(crate) fn new(
        graphics_device: &Arc<GraphicsDeviceShared>,
        descriptor: &GraphicsPipelineDescriptor,
    ) -> Self {
        let layout = descriptor.layout.downcast_ref::<PipelineLayout>().unwrap();

        let vertex_shader_code = descriptor
            .vertex_shader
            .downcast_ref::<ShaderModule>()
            .unwrap()
            .code();

        let fragment_shader_code = descriptor
            .fragment_shader
            .downcast_ref::<ShaderModule>()
            .unwrap()
            .code();
        let mut state_descriptor = D3D12_GRAPHICS_PIPELINE_STATE_DESC {
            pRootSignature: unsafe { mem::transmute_copy(layout.raw()) },
            VS: D3D12_SHADER_BYTECODE {
                pShaderBytecode: vertex_shader_code.as_ptr() as *const c_void,
                BytecodeLength: vertex_shader_code.len(),
            },
            PS: D3D12_SHADER_BYTECODE {
                pShaderBytecode: fragment_shader_code.as_ptr() as *const c_void,
                BytecodeLength: fragment_shader_code.len(),
            },
            RasterizerState: D3D12_RASTERIZER_DESC {
                FillMode: D3D12_FILL_MODE_SOLID,
                CullMode: D3D12_CULL_MODE_NONE,
                ..Default::default()
            },
            BlendState: D3D12_BLEND_DESC {
                AlphaToCoverageEnable: false.into(),
                IndependentBlendEnable: false.into(),
                RenderTarget: [
                    D3D12_RENDER_TARGET_BLEND_DESC {
                        BlendEnable: false.into(),
                        LogicOpEnable: false.into(),
                        SrcBlend: D3D12_BLEND_ONE,
                        DestBlend: D3D12_BLEND_ZERO,
                        BlendOp: D3D12_BLEND_OP_ADD,
                        SrcBlendAlpha: D3D12_BLEND_ONE,
                        DestBlendAlpha: D3D12_BLEND_ZERO,
                        BlendOpAlpha: D3D12_BLEND_OP_ADD,
                        LogicOp: D3D12_LOGIC_OP_NOOP,
                        RenderTargetWriteMask: D3D12_COLOR_WRITE_ENABLE_ALL.0 as u8,
                    },
                    D3D12_RENDER_TARGET_BLEND_DESC::default(),
                    D3D12_RENDER_TARGET_BLEND_DESC::default(),
                    D3D12_RENDER_TARGET_BLEND_DESC::default(),
                    D3D12_RENDER_TARGET_BLEND_DESC::default(),
                    D3D12_RENDER_TARGET_BLEND_DESC::default(),
                    D3D12_RENDER_TARGET_BLEND_DESC::default(),
                    D3D12_RENDER_TARGET_BLEND_DESC::default(),
                ],
            },
            DepthStencilState: D3D12_DEPTH_STENCIL_DESC::default(),
            SampleMask: u32::MAX,
            PrimitiveTopologyType: D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
            NumRenderTargets: 1,
            SampleDesc: DXGI_SAMPLE_DESC {
                Count: 1,
                ..Default::default()
            },
            ..Default::default()
        };
        state_descriptor.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

        let pipeline_state: ID3D12PipelineState = unsafe {
            graphics_device
                .device()
                .CreateGraphicsPipelineState(&state_descriptor)
        }
        .unwrap();

        if let Some(label) = descriptor.label {
            graphics_device.set_debug_name(&pipeline_state, label);
        }

        Self {
            raw: ManuallyDrop::new(pipeline_state),

            layout: Arc::clone(descriptor.layout),

            graphics_device: Arc::clone(graphics_device),
        }
    }

    pub(crate) fn layout(&self) -> &Arc<dyn crate::pipeline_layout::PipelineLayout> {
        &self.layout
    }

    pub(crate) fn raw(&self) -> &ID3D12PipelineState {
        &self.raw
    }
}

impl Drop for GraphicsPipeline {
    fn drop(&mut self) {
        self.graphics_device
            .resource_queue()
            .push_graphics_pipeline(unsafe { ManuallyDrop::take(&mut self.raw) });
    }
}

impl Debug for GraphicsPipeline {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        f.debug_struct("GraphicsPipeline")
            .field("raw", &self.raw)
            .field("layout", &self.layout)
            .finish()
    }
}

impl crate::graphics_pipeline::GraphicsPipeline for GraphicsPipeline {}
