/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "HyperEngine/Support/Expected.hpp"
#include "HyperEngine/Support/Prerequisites.hpp"

#include <sstream>
#include <string>
#include <string_view>

namespace HyperEngine::Formatter
{
	namespace Detail
	{
		template <typename T>
		concept Printable = requires(std::ostream &ostream, T value)
		{
			ostream << value;
		};

		inline void format(std::stringstream &, size_t)
		{
		}

		template <Printable T, Printable... Args>
		void format(
			std::stringstream &stream,
			size_t argument_index,
			T &&argument,
			Args &&...args)
		{
			if (argument_index != 0)
			{
				format(stream, argument_index - 1, std::forward<Args>(args)...);
				return;
			}

			stream << argument;
		}
	} // namespace Detail

	template <typename... Args>
	Expected<std::string> format(std::string_view format, Args &&...args)
	{
		std::stringstream stream;

		size_t argument_index = 0;
		for (auto iterator = format.cbegin(); iterator != format.cend(); ++iterator)
		{
			if (*iterator != '{')
			{
				stream << *iterator;
				continue;
			}

			++iterator;

			if (*iterator != '}')
			{
				return Error("format bracket unclosed");
			}

			constexpr size_t argument_size = sizeof...(args);
			if (argument_index >= argument_size)
			{
				return Error("format argument not found");
			}

			Detail::format(stream, argument_index++, std::forward<Args>(args)...);
		}

		return stream.str();
	}
} // namespace HyperEngine::Formatter
