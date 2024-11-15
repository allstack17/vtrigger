#include <string>
#include <fstream>

#include "triggerbot.h"
#include "common.h"
#include "nlohmann/json.hpp"

ConfigFileData* ReadConfigFile() noexcept
{
	std::ifstream file("config.json");
	if (!file)
		_exit_msg("no config file");

	ConfigFileData* cfg = new ConfigFileData;

	try {
		nlohmann::json json;
		file >> json;

		cfg->_hotkey = std::stoi(
			std::string(json["hotkey"]).c_str(), 
			nullptr, 16);

		cfg->_second_key = std::stoi(
			std::string(json["second_key"]).c_str(),
			nullptr, 16);

		cfg->_delay	    = json["delay"];
		cfg->_always_enable = json["always_enable"];
		cfg->_zone_x	    = json["zone_x"];
		cfg->_zone_y	    = json["zone_y"];
		cfg->_tolerance	    = json["tolerance"];
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << '\n';
		_exit_msg("json file error");
	}

	std::cout
		<< "hotkey:\t\t0x"    << std::hex << (int)cfg->_hotkey << '\n'
		<< "second key:\t0x"  << (int)cfg->_second_key << '\n'
		<< "delay:\t\t"	      << std::dec << cfg->_delay << '\n'
		<< "always enable:\t" << std::boolalpha << cfg->_always_enable << '\n'
		<< "zone:\t\t"	      << cfg->_zone_x << " : " << cfg->_zone_y << '\n'
		<< "tolerance:\t"     << cfg->_tolerance << '\n';

	file.close();
	return cfg;
}
