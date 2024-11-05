#include "triggerbot.h"
#include "common.h"

#include <string>
#include <fstream>

ConfigFileData* ReadConfigFile()
{
	std::ifstream file("config.txt");
	if (!file)
		_exit_msg("no config file");

	auto foo = [](std::string src) -> char* {
		return &(src[src.find('=') + 1]);
	};

	ConfigFileData* cfg = new ConfigFileData;

	std::string str; {
		std::getline(file, str); cfg->_hotkey	     = std::stoi(foo(str), nullptr, 16);
		std::getline(file, str); cfg->_delay	     = atoi(foo(str));
		std::getline(file, str); cfg->_second_key    = std::stoi(foo(str), nullptr, 16);
		std::getline(file, str); cfg->_always_enable = atoi(foo(str));
		std::getline(file, str); cfg->_dots_distance = atoi(foo(str));
		std::getline(file, str); cfg->_tolerance	 = atoi(foo(str));
	}

#if 1
	std::cout << "config:\n"
		<< "\tsecond_key:\t"	  << (int)cfg->_second_key
		<< "\n\tdelay:\t\t"		  << cfg->_delay
		<< "\n\thotkey:\t\t"	  << (int)cfg->_hotkey
		<< "\n\talways_enable:\t" << cfg->_always_enable
		<< "\n\tzone:\t\t"		  << cfg->_dots_distance
		<< "\n\ttolerance:\t"	  << cfg->_tolerance << '\n';
#endif

	file.close();
	return cfg;
}
