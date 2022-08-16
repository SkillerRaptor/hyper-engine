/*
 * Copyright (c) 2022-present, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

use crate::core::window::Window;
use crate::rendering::device::Device;
use crate::rendering::error::Error;
use crate::rendering::instance::Instance;
use crate::rendering::pipeline::Pipeline;
use crate::rendering::surface::Surface;
use crate::rendering::swapchain::Swapchain;

use log::info;

// NOTE: Using Rc for ref-counting, replace with Arc when multithreading
pub struct RenderContext {
    pub pipeline: std::rc::Rc<Pipeline>,
    pub swapchain: std::rc::Rc<Swapchain>,
    pub device: std::rc::Rc<Device>,
    pub surface: std::rc::Rc<Surface>,
    pub instance: std::rc::Rc<Instance>,
    _entry: ash::Entry,
}

impl RenderContext {
    pub fn new(window: &Window) -> Result<Self, Error> {
        let entry = unsafe { ash::Entry::load()? };
        let instance = std::rc::Rc::new(Instance::new(&window, &entry)?);
        let surface = std::rc::Rc::new(Surface::new(&window, &entry, &instance)?);
        let device = std::rc::Rc::new(Device::new(&instance, &surface)?);
        let swapchain = std::rc::Rc::new(Swapchain::new(&window, &instance, &surface, &device)?);
        let pipeline = std::rc::Rc::new(Pipeline::new(&device, &swapchain)?);

        info!("Created render context");
        Ok(Self {
            pipeline,
            swapchain,
            device,
            surface,
            instance,
            _entry: entry,
        })
    }
}
