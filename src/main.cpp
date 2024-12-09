#include <fstream>
#include <thread>
#include <ShlObj_core.h>

#include "triggerbot.h"
#include "gui.h"
#include "common.h"

/* src: triggerbot.cpp */
extern volatile std::atomic<bool> TRIGGER_MAIN_LOOP;

#define CONFIG_FILE_UPDATE() {\
	SetConfigFileData("second_key", cfg._second_key);\
	SetConfigFileData("hotkey",     cfg._hotkey);\
	SetConfigFileData("delay",      cfg._delay);\
	SetConfigFileData("zone_x",     cfg._zone_x);\
	SetConfigFileData("zone_y",     cfg._zone_y);\
	SetConfigFileData("tolerance",  cfg._tolerance);\
}

static bool EarlyInit();

int main(int argc, char** argv) 
{
	if (!EarlyInit())
		__exit_msg("restart with admin rights");

	ShowWindow(GetConsoleWindow(), SW_SHOW);

	auto &cfg = *ReadConfigFile();
	std::thread trigger(TriggerThread, std::ref(cfg));

	if (!GuiMain(cfg)) {
		__set_color_exp(SC_COLOR_RED,
			std::cout << "[gui failed]");
		std::cout << " => console\n\nrunning...\n";
	}
	else {
		CONFIG_FILE_UPDATE();
		trigger.detach();
		return 0;
	}

	TRIGGER_MAIN_LOOP = true;
	trigger.join();

	return 0;
}

bool EarlyInit() 
{
	MoveWindow(
		GetConsoleWindow(),
		500, 500, 500, 300,
		true
	);
	SetWindowTextA(GetConsoleWindow(), "Trigger");

	return IsUserAnAdmin() ? true : false;
}