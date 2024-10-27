/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include <exception>
#include <string>

#include <argparse/argparse.hpp>

#include <hyper_core/logger.hpp>
#include <hyper_rhi/graphics_device.hpp>

#include "hyper_engine/engine.hpp"

int main(int argc, char **argv)
{
    hyper_core::logger::initialize();

    argparse::ArgumentParser program("HyperEngine");

    std::string log_level = "info";
    program.add_argument("--level")
        .default_value("info")
        .choices("info", "warning", "error", "critical", "debug", "trace")
        .store_into(log_level);

    uint32_t width = 0;
    program.add_argument("--width").default_value(static_cast<uint32_t>(1280)).scan<'i', uint32_t>().store_into(width);

    uint32_t height = 0;
    program.add_argument("--height").default_value(static_cast<uint32_t>(720)).scan<'i', uint32_t>().store_into(height);

    std::string renderer = "vulkan";
    program.add_argument("--renderer").default_value("vulkan").choices("d3d12", "vulkan").store_into(renderer);

    bool debug = false;
    program.add_argument("--debug").default_value(false).implicit_value(true).store_into(debug);

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &error)
    {
        HE_ERROR("{}", error.what());
        return 1;
    }

    if (log_level == "trace")
    {
        hyper_core::logger::set_level(spdlog::level::trace);
    }
    else if (log_level == "debug")
    {
        hyper_core::logger::set_level(spdlog::level::debug);
    }
    else if (log_level == "info")
    {
        hyper_core::logger::set_level(spdlog::level::info);
    }
    else if (log_level == "warning")
    {
        hyper_core::logger::set_level(spdlog::level::warn);
    }
    else if (log_level == "error")
    {
        hyper_core::logger::set_level(spdlog::level::err);
    }
    else if (log_level == "critical")
    {
        hyper_core::logger::set_level(spdlog::level::critical);
    }

    const hyper_rhi::GraphicsApi graphics_api = renderer == "d3d12" ? hyper_rhi::GraphicsApi::D3D12 : hyper_rhi::GraphicsApi::Vulkan;

    auto engine = hyper_engine::Engine({
        .width = width,
        .height = height,
        .graphics_api = graphics_api,
        .debug = debug,
    });
    engine.run();

    return 0;
}
