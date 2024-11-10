/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/render_passes/imgui_pass.hpp"

#include <imgui.h>
#include <imgui_impl_sdl3.h>

namespace hyper_render
{
    ImGuiPass::ImGuiPass(
        hyper_event::EventBus &event_bus,
        const hyper_platform::Window &window,
        const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device,
        const std::shared_ptr<hyper_rhi::Surface> &surface)
        : m_imgui_manager(graphics_device->create_imgui_manager())
    {
        event_bus.subscribe<hyper_platform::SdlEvent>(ImGuiPass::on_sdl_event);

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
        const std::shared_ptr<hyper_rhi::CommandList> &command_list,
        const std::shared_ptr<hyper_rhi::TextureView> &swapchain_texture_view) const
    {
        m_imgui_manager->new_frame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();

        const std::shared_ptr<hyper_rhi::RenderPass> render_pass = command_list->begin_render_pass({
            .label = "ImGui",
            .label_color =
                hyper_rhi::LabelColor{
                    .red = 17,
                    .green = 255,
                    .blue = 170,
                },
            .color_attachments = {
                hyper_rhi::ColorAttachment{
                    .view = swapchain_texture_view,
                    .operation =
                        hyper_rhi::Operations{
                            .load_operation = hyper_rhi::LoadOperation::Load,
                            .store_operation = hyper_rhi::StoreOperation::Store,
                        },
                },
            },
            .depth_stencil_attachment =
                hyper_rhi::DepthStencilAttachment{
                    .view = nullptr,
                    .depth_operation =
                        hyper_rhi::Operations{
                            .load_operation = hyper_rhi::LoadOperation::DontCare,
                            .store_operation = hyper_rhi::StoreOperation::DontCare,
                        },
                },
            });

        m_imgui_manager->render(command_list);
    }

    void ImGuiPass::on_sdl_event(const hyper_platform::SdlEvent &event)
    {
        ImGui_ImplSDL3_ProcessEvent(event.event());
    }
} // namespace hyper_render