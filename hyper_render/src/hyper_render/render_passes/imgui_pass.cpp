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

namespace hyper_engine
{
    ImGuiPass::ImGuiPass(
        EventBus &event_bus,
        const Window &window,
        const std::shared_ptr<IGraphicsDevice> &graphics_device,
        const std::shared_ptr<ISurface> &surface)
        : m_imgui_manager(graphics_device->create_imgui_manager())
    {
        event_bus.subscribe<SdlEvent>(ImGuiPass::on_sdl_event);

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
        const std::shared_ptr<ICommandList> &command_list,
        const std::shared_ptr<ITextureView> &swapchain_texture_view) const
    {
        m_imgui_manager->new_frame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();

        const std::shared_ptr<IRenderPass> render_pass = command_list->begin_render_pass({
            .label = "ImGui",
            .label_color =
                LabelColor{
                    .red = 17,
                    .green = 255,
                    .blue = 170,
                },
            .color_attachments = {
                ColorAttachment{
                    .view = swapchain_texture_view,
                    .operation =
                        Operations{
                            .load_operation = LoadOperation::Load,
                            .store_operation = StoreOperation::Store,
                        },
                },
            },
            .depth_stencil_attachment =
                DepthStencilAttachment{
                    .view = nullptr,
                    .depth_operation =
                        Operations{
                            .load_operation = LoadOperation::DontCare,
                            .store_operation = StoreOperation::DontCare,
                        },
                },
            });

        m_imgui_manager->render(command_list);
    }

    void ImGuiPass::on_sdl_event(const SdlEvent &event)
    {
        ImGui_ImplSDL3_ProcessEvent(event.event());
    }
} // namespace hyper_engine