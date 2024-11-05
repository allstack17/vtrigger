#include "triggerbot.h"
#include "common.h"

int main(int argc, char** argv)
{
	MoveWindow(
		GetConsoleWindow(), 
		500, 500, 500, 400, 
		true
	);
	SetWindowTextA(GetConsoleWindow(), "TriggerBot");

	ConfigFileData* cfg = ReadConfigFile();

	TriggerBot bot(cfg);

	std::cout << "\n\nrunning...\n";
	while (1) {
		if (cfg->_always_enable) {
			if (bot.check_screen()) {
				bot.click(cfg->_second_key);
				sleep(cfg->_delay);
			}
		}
		else {
			if (GetAsyncKeyState(cfg->_hotkey)) {
				if (bot.check_screen()) {
					bot.click(cfg->_second_key);
					sleep(cfg->_delay);
				}
			}
		}
		sleep(1);
	}
}