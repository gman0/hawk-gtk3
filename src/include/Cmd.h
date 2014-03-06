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
