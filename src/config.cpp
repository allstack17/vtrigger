#include <string>
#include <fstream>
#include <string>

#include "triggerbot.h"
#include "common.h"
#include "nlohmann/json.hpp"

/* default config
 *
 * {
 * 	"delay": 40,
 * 	"hotkey": 164,		// 0xA4
 * 	"second_key": 75,   // 0x4B
 * 	"tolerance": 70,k
 * 	"zone_x": 5,
 * 	"zone_y": 6
 * }
 *
 */

namespace global {
	nlohmann::json json;
}

ConfigFileData* ReadConfigFile() noexcept {
	std::ifstream file("config.json");
	if (!file)
		__exit_msg("no config file");

	ConfigFileData* cfg = new ConfigFileData;

	try {
		/* json init */
		file >> global::json;
		file.close();

		cfg->_second_key = global::json["second_key"];
		cfg->_hotkey	 = global::json["hotkey"];
		cfg->_delay		 = global::json["delay"];
		cfg->_zone_x	 = global::json["zone_x"];
		cfg->_zone_y	 = global::json["zone_y"];
		cfg->_tolerance	 = global::json["tolerance"];
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << '\n';
		__exit_msg("json file error");
	}

	/* dbg */
#if 0
	std::cout
		<< "hotkey:\t\t0x"    << std::hex << (int)cfg->_hotkey << '\n'
		<< "second key:\t0x"  << (int)cfg->_second_key << '\n'
		<< "delay:\t\t"	      << std::dec << cfg->_delay << '\n'
		<< "zone:\t\t"	      << cfg->_zone_x << " : " << cfg->_zone_y << '\n'
		<< "tolerance:\t"     << cfg->_tolerance << "\n\n";
#endif

	return cfg;
}

void SetConfigFileData(const std::string& field, const int& data) noexcept {
	std::ofstream ofile("config.json", std::ios::trunc);
	if (!ofile)
		__exit_msg("no config file");

	try {
		global::json[field] = data;
		ofile << global::json.dump(4);
		ofile.close();
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << '\n';
		__exit_msg("json file error");
	}
}