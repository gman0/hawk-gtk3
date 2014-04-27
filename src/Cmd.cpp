/*
	Copyright (C) 2013-2014 Róbert "gman" Vašek <gman@codefreax.org>

	This file is part of hawk-gtk3.

	hawk-gtk3 is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	hawk-gtk3 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with hawk-gtk3.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <utility>
#include "Cmd.h"

void Cmd::register_command(std::string&& cmd_name, Command&& cmd)
{
	m_commands[std::move(cmd_name)] = std::move(cmd);
}

bool Cmd::operator()(const std::string& cmd_string)
{
	if (cmd_string.empty())
		return true;

	static std::string cmd_name, cmd_args;
	parse(cmd_string, cmd_name, cmd_args);

	auto it = m_commands.find(cmd_name);

	if (it == m_commands.end())
		return false;

	return it->second(cmd_args);
}

void Cmd::parse(const std::string& cmd_string, std::string& cmd_name,
	std::string& cmd_args)
{
	std::string::size_type space_pos = cmd_string.find_first_of(' ');

	cmd_name = cmd_string.substr(0, space_pos);

	if (space_pos != std::string::npos)
		cmd_args = cmd_string.substr(++space_pos, cmd_string.size());
}
