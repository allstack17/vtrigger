#include "triggerbot.h"
#include "common.h"

int main(int argc, char** argv)
{
	MoveWindow(
		GetConsoleWindow(), 
		500, 500, 600, 400, 
		true
	);
	SetWindowTextA(GetConsoleWindow(), "TriggerBot");

	ConfigFileData* cfg = ReadConfigFile();
	TriggerBot bot(cfg);

#ifdef __SIMD
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	std::cout << "\nused SIMD: ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << "running...\n";
#else
	std::cout << "\nrunning...\n";
#endif

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
	
	return 0;
}