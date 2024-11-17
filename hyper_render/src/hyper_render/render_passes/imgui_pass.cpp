/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/render_passes/imgui_pass.hpp"

#include <imgui.h>
#include <imgui_impl_sdl3.h>

#include <hyper_platform/sdl_event.hpp>
#include <hyper_rhi/command_list.hpp>
#include <hyper_rhi/graphics_device.hpp>
#include <hyper_rhi/imgui_manager.hpp>
#include <hyper_rhi/render_pass.hpp>
#include <hyper_rhi/texture_view.hpp>

#include <hyper_event/event_bus.hpp>

namespace he::render
{
    ImGuiPass::ImGuiPass(
        he::event::EventBus &event_bus,
        const he::platform::Window &window,
        const std::shared_ptr<he::rhi::GraphicsDevice> &graphics_device,
        const std::shared_ptr<he::rhi::Surface> &surface)
        : m_imgui_manager(graphics_device->create_imgui_manager())
    {
        event_bus.subscribe<he::platform::SdlEvent>(ImGuiPass::on_sdl_event);

        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_IsSRGB;

        ImGui::StyleColorsDark();

        m_imgui_manager->initialize(window, surface);
    }

    ImGuiPass::~ImGuiPass()
    {
        m_imgui_manager->destroy();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiPass::render(
        const std::shared_ptr<he::rhi::CommandList> &command_list,
        const std::shared_ptr<he::rhi::TextureView> &swapchain_texture_view) const
    {
        m_imgui_manager->new_frame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();

        const std::shared_ptr<he::rhi::RenderPass> render_pass = command_list->begin_render_pass({
            .label = "ImGui",
            .label_color =
                he::rhi::LabelColor{
                    .red = 17,
                    .green = 255,
                    .blue = 170,
                },
            .color_attachments = {
                he::rhi::ColorAttachment{
                    .view = swapchain_texture_view,
                    .operation =
                        he::rhi::Operations{
                            .load_operation = he::rhi::LoadOperation::Load,
                            .store_operation = he::rhi::StoreOperation::Store,
                        },
                },
            },
            .depth_stencil_attachment =
                he::rhi::DepthStencilAttachment{
                    .view = nullptr,
                    .depth_operation =
                        he::rhi::Operations{
                            .load_operation = he::rhi::LoadOperation::DontCare,
                            .store_operation = he::rhi::StoreOperation::DontCare,
                        },
                },
            });

        m_imgui_manager->render(command_list);
    }

    void ImGuiPass::on_sdl_event(const he::platform::SdlEvent &event)
    {
        ImGui_ImplSDL3_ProcessEvent(event.event());
    }
} // namespace he::render