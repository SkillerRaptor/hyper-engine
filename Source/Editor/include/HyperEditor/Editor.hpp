/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "HyperEditor/DiscordPresence.hpp"

#include <HyperEngine/IApplication.hpp>

namespace HyperEditor
{
	class Editor final : public HyperEngine::IApplication
	{
	public:
		Editor();

	private:
		auto startup() -> void override;
		auto shutdown() -> void override;
		
		auto update() -> void override;
		
	private:
		DiscordPresence m_discord_presence{ "HyperEditor" };
	};
} // namespace HyperEditor
