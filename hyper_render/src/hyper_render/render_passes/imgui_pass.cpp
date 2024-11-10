/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_render/render_passes/imgui_pass.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>

namespace hyper_render
{
    ImGuiPass::ImGuiPass(
        const hyper_platform::Window &window,
        const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device,
        const std::shared_ptr<hyper_rhi::Surface> &surface)
        : m_imgui_manager(graphics_device->create_imgui_manager())
    {
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_IsSRGB;

        ImGui::StyleColorsDark();

        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        m_imgui_manager->initialize(window, surface);
    }

    ImGuiPass::~ImGuiPass()
    {
        m_imgui_manager->destroy();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiPass::render(
        const std::shared_ptr<hyper_rhi::CommandList> &command_list,
        const std::shared_ptr<hyper_rhi::TextureView> &swapchain_texture_view) const
    {
        m_imgui_manager->new_frame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();

        {
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

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
} // namespace hyper_render