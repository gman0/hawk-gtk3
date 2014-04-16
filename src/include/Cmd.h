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

#ifndef CMD_H
#define CMD_H

#include <string>
#include <unordered_map>
#include <functional>

class Cmd
{
public:
	using Command = std::function<bool(const std::string&)>;

private:
	std::unordered_map<std::string, Command> m_commands;

public:
	void register_command(std::string&& cmd_name, Command&& cmd);
	bool operator()(const std::string& cmd_name);

private:
	void parse(const std::string& cmd_string, std::string& cmd_name,
		std::string& cmd_args);
};

#endif // CMD_H
