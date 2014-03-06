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
	cmd_args = cmd_string.substr(++space_pos, cmd_string.size());
}
