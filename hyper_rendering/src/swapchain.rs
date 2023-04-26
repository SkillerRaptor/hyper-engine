/*
 * Copyright (c) 2023, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

use std::sync::Arc;

use crate::{
    device::{
        queue_family_indices::{self, QueueFamilyIndices},
        swapchain_support_details::{self, SwapchainSupportDetails},
        Device,
    },
    instance::Instance,
    surface::Surface,
};

use hyper_platform::window::Window;

use ash::{
    extensions::khr,
    vk::{
        self, ColorSpaceKHR, ComponentMapping, ComponentSwizzle, CompositeAlphaFlagsKHR, Extent2D,
        Format, Image, ImageAspectFlags, ImageSubresourceRange, ImageUsageFlags, ImageView,
        ImageViewCreateInfo, ImageViewType, PresentModeKHR, SharingMode, SurfaceCapabilitiesKHR,
        SurfaceFormatKHR, SwapchainCreateInfoKHR, SwapchainKHR,
    },
};
use thiserror::Error;

/// An enum representing the errors that can occur while constructing a swapchain
#[derive(Debug, Error)]
pub enum CreationError {
    /// Creation of a vulkan object failed
    #[error("creation of vulkan {1} failed")]
    Creation(#[source] vk::Result, &'static str),

    /// Device out of memory
    #[error("device is out of memory")]
    OutOfMemory(#[source] vk::Result),

    /// Creation of a queue family indices failed
    #[error("creation of queue family indices failed")]
    QueueFamilyIndicesFailure(#[from] queue_family_indices::CreationError),

    /// Creation of a swapchain support details failed
    #[error("creation of swapchain support details failed")]
    SwapchainSupportDetailsFailure(#[from] swapchain_support_details::CreationError),
}

/// A struct representing a wrapper for the vulkan swapchain
pub(crate) struct Swapchain {
    /// Image view handles of the images
    image_views: Vec<ImageView>,

    /// Image handles of the swapchain
    _images: Vec<Image>,

    /// Swapchain extent
    _extent: Extent2D,

    /// Swapchain format
    _format: Format,

    /// Vulkan swapchain handle
    handle: SwapchainKHR,

    /// Ash swapchain loader
    swapchain_loader: khr::Swapchain,

    /// Device Wrapper
    device: Arc<Device>,
}

impl Swapchain {
    /// Constructs a new swapchain
    pub(crate) fn new(
        window: &Window,
        instance: &Instance,
        surface: &Surface,
        device: &Arc<Device>,
    ) -> Result<Self, CreationError> {
        let swapchain_support_details =
            SwapchainSupportDetails::new(surface, device.physical_device())?;

        let capabilities = swapchain_support_details.capabilities();

        let extent = Self::choose_extent(window, &capabilities);
        let surface_format = Self::choose_surface_format(swapchain_support_details.formats());
        let present_mode = Self::choose_present_mode(swapchain_support_details.present_modes());

        let mut image_count = capabilities.min_image_count + 1;
        if capabilities.max_image_count > 0 && image_count > capabilities.max_image_count {
            image_count = capabilities.max_image_count;
        }

        let queue_family_indices =
            QueueFamilyIndices::new(instance, surface, device.physical_device())?;
        let queue_families = [
            queue_family_indices.graphics_family().unwrap(),
            queue_family_indices.present_family().unwrap(),
        ];

        let (image_sharing_mode, queue_family_indices_array): (SharingMode, &[u32]) =
            if queue_families[0] != queue_families[1] {
                (SharingMode::CONCURRENT, &queue_families)
            } else {
                (SharingMode::EXCLUSIVE, &[])
            };

        let swapchain_create_info = SwapchainCreateInfoKHR::builder()
            .surface(*surface.handle())
            .min_image_count(image_count)
            .image_format(surface_format.format)
            .image_color_space(surface_format.color_space)
            .image_extent(extent)
            .image_array_layers(1)
            .image_usage(ImageUsageFlags::COLOR_ATTACHMENT)
            .image_sharing_mode(image_sharing_mode)
            .queue_family_indices(queue_family_indices_array)
            .pre_transform(capabilities.current_transform)
            .composite_alpha(CompositeAlphaFlagsKHR::OPAQUE)
            .present_mode(present_mode)
            .clipped(true)
            .old_swapchain(SwapchainKHR::null());

        let swapchain_loader = khr::Swapchain::new(instance.handle(), device.handle());

        let swapchain = unsafe {
            swapchain_loader
                .create_swapchain(&swapchain_create_info, None)
                .map_err(|error| CreationError::Creation(error, "swapchain"))
        }?;

        let swapchain_images = unsafe {
            swapchain_loader
                .get_swapchain_images(swapchain)
                .map_err(CreationError::OutOfMemory)
        }?;

        let mut swapchain_image_views = Vec::new();
        for image in &swapchain_images {
            let component_mapping = ComponentMapping::builder()
                .r(ComponentSwizzle::IDENTITY)
                .g(ComponentSwizzle::IDENTITY)
                .b(ComponentSwizzle::IDENTITY)
                .a(ComponentSwizzle::IDENTITY);

            let subresource_range = ImageSubresourceRange::builder()
                .aspect_mask(ImageAspectFlags::COLOR)
                .base_mip_level(0)
                .level_count(1)
                .base_array_layer(0)
                .layer_count(1);

            let image_view_create_info = ImageViewCreateInfo::builder()
                .image(*image)
                .view_type(ImageViewType::TYPE_2D)
                .format(surface_format.format)
                .components(*component_mapping)
                .subresource_range(*subresource_range);

            let image_view = unsafe {
                device
                    .handle()
                    .create_image_view(&image_view_create_info, None)
                    .map_err(|error| CreationError::Creation(error, "image view"))
            }?;

            swapchain_image_views.push(image_view);
        }

        Ok(Self {
            image_views: swapchain_image_views,
            _images: swapchain_images,
            _extent: extent,
            _format: surface_format.format,
            handle: swapchain,
            swapchain_loader,
            device: device.clone(),
        })
    }

    /// Chooses the right window extent
    ///
    /// Arguments:
    ///
    /// * `window`: Application window
    /// * `capabilities`: Surface capabilities
    fn choose_extent(window: &Window, capabilities: &SurfaceCapabilitiesKHR) -> Extent2D {
        if capabilities.current_extent.width != u32::MAX
            || capabilities.current_extent.height != u32::MAX
        {
            return capabilities.current_extent;
        }

        let framebuffer_size = window.framebuffer_size();

        Extent2D {
            width: framebuffer_size.0.clamp(
                capabilities.min_image_extent.width,
                capabilities.max_image_extent.width,
            ),
            height: framebuffer_size.1.clamp(
                capabilities.min_image_extent.height,
                capabilities.max_image_extent.height,
            ),
        }
    }

    /// Chooses the right surface format
    ///
    /// Arguments:
    ///
    /// * `formats`: Surface formats
    fn choose_surface_format(formats: &[SurfaceFormatKHR]) -> SurfaceFormatKHR {
        *formats
            .iter()
            .find(|&format| {
                format.format == Format::B8G8R8A8_SRGB
                    && format.color_space == ColorSpaceKHR::SRGB_NONLINEAR
            })
            .unwrap_or_else(|| &formats[0])
    }

    /// Chooses the right present mode
    ///
    /// Arguments:
    ///
    /// * `present_modes`: Surface present modes
    fn choose_present_mode(present_modes: &[PresentModeKHR]) -> PresentModeKHR {
        *present_modes
            .iter()
            .find(|&present_mode| *present_mode == PresentModeKHR::MAILBOX)
            .unwrap_or(&PresentModeKHR::FIFO)
    }
}

impl Drop for Swapchain {
    fn drop(&mut self) {
        unsafe {
            for image_view in &self.image_views {
                self.device.handle().destroy_image_view(*image_view, None);
            }

            self.swapchain_loader.destroy_swapchain(self.handle, None);
        }
    }
}
